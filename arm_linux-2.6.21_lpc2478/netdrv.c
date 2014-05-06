/*
 * LPC2478 + KSZ8041NL(net) driver 
 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include <linux/io.h>
#include <linux/uaccess.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/delay.h>

#include "lpc2478_net.h"

#define NET_IRQ   21

#define NET_TIMEOUT (5000*100)/1000//1000

#define NET_BASE_ADDR 0xFFE00000
#define NET_MEM_LEN   0x1000

#define DEV_NAME     "ksz8041"

void __iomem *net_io_base_addr;

const char default_net_mac[] = {0x00,0x14,0x97,0x0F,0x78,0x71};

struct tasklet_struct	tasklet;

static uint16_t PHY_read(uint8_t paddr,uint8_t raddr)
{
	uint16_t temp16;
	uint8_t i;
	
	iowrite32(0x01,net_io_base_addr + rMCMD);					//enable read 
	iowrite32(((paddr&0X1F) <<8 ) + (raddr&0X1F),net_io_base_addr + rMADR);
	
	/*for (i = 0; i < 10; ++i)
	{
		if ((ioread32(net_io_base_addr + rMIND) & 0x01) == 0)
			break;
		udelay(10);
	}*/
	while((ioread32(net_io_base_addr + rMIND) & 0x01));
	
	iowrite32(0x00,net_io_base_addr + rMCMD);
	
	temp16 = ioread32(net_io_base_addr + rMRDD);
	return temp16;	
}


static void PHY_write(uint8_t paddr,uint8_t raddr,uint32_t val)
{
	int i;
	iowrite32(0x0,net_io_base_addr + rMCMD);
	iowrite32((paddr << 8) | raddr, net_io_base_addr + rMADR);
	iowrite32(val,net_io_base_addr + rMWTD);
	
	while(ioread32(net_io_base_addr + rMIND) & 0x01);
	/*for (i = 0; i < 10; ++i)
	{
		if (ioread32(net_io_base_addr + rMIND) == 0)
			break;
		
		udelay(10);
	}*/
}


static void EMAC_enable_tx(void)
{
	iowrite32(ioread32(net_io_base_addr + rECOMMAND) | 0x02,net_io_base_addr + rECOMMAND);
}

static void EMAC_disable_tx(void)
{
	iowrite32(ioread32(net_io_base_addr + rECOMMAND) & (~0x02),net_io_base_addr + rECOMMAND);
}

static void EMAC_enable_rx(void)
{
	iowrite32(ioread32(net_io_base_addr + rECOMMAND) | 0x01,net_io_base_addr + rECOMMAND);
	iowrite32(ioread32(net_io_base_addr + rMAC1) | 0x01,net_io_base_addr + rMAC1);
}

static void EMAC_disable_rx(void)
{
	iowrite32(ioread32(net_io_base_addr + rECOMMAND) & (~0x01),net_io_base_addr + rECOMMAND);
	iowrite32(ioread32(net_io_base_addr + rMAC1) & (~0x01),net_io_base_addr + rMAC1);
}


static void EMAC_tx_descriptor_init( void )
{
    uint32_t i;
    uint32_t *tx_desc_addr, *tx_status_addr;
   
    /*-----------------------------------------------------------------------------      
     * setup the Tx status,descriptor registers -- 
     * Note, the actual tx packet data is loaded into the ahb2_sram16k memory as part
     * of the simulation
     *----------------------------------------------------------------------------*/ 
	iowrite32(TX_DESCRIPTOR_ADDR,net_io_base_addr + rTXDESCR);
	iowrite32(TX_STATUS_ADDR,net_io_base_addr + rTXSTATU);
	iowrite32(EMAC_TX_DESCRIPTOR_COUNT - 1, net_io_base_addr + rTXDENUM);

    for ( i = 0; i < EMAC_TX_DESCRIPTOR_COUNT; i++ )
    {
		tx_desc_addr = (uint32_t *)(TX_DESCRIPTOR_ADDR + i * 8);	/* two words at a time, packet and control */
		*tx_desc_addr = (uint32_t)(EMAC_TX_BUFFER_ADDR + i * EMAC_BLOCK_SIZE);
		*(tx_desc_addr+1) = (uint32_t)(EMAC_TX_DESC_INT | (EMAC_BLOCK_SIZE - 1));	/* set size only */
    }
    
    for ( i = 0; i < EMAC_TX_DESCRIPTOR_COUNT; i++ )
    {
		tx_status_addr = (uint32_t *)(TX_STATUS_ADDR + i * 4);	/* TX status, one word only, status info. */
		*tx_status_addr = (uint32_t)0;		/* initially, set status info to 0 */
    }
	
	iowrite32(0x0,net_io_base_addr + rTXPROIN);
}

void EMAC_rx_descriptor_init( void )
{
    uint32_t i;
    uint32_t *rx_desc_addr, *rx_status_addr;
   
    /*-----------------------------------------------------------------------------      
     * setup the Rx status,descriptor registers -- 
     * Note, the actual rx packet data is loaded into the ahb2_sram16k memory as part
     * of the simulation
     *----------------------------------------------------------------------------*/ 
    
	iowrite32(RX_DESCRIPTOR_ADDR,net_io_base_addr + rRXDESCR);
	iowrite32(RX_STATUS_ADDR,net_io_base_addr + rRXSTATU);
	iowrite32(EMAC_RX_DESCRIPTOR_COUNT - 1,net_io_base_addr + rRXDENUM);

    for ( i = 0; i < EMAC_RX_DESCRIPTOR_COUNT; i++ )
    {
		/* two words at a time, packet and control */
		rx_desc_addr = (uint32_t *)(RX_DESCRIPTOR_ADDR + i * 8);
		*rx_desc_addr = (uint32_t)(EMAC_RX_BUFFER_ADDR + i * EMAC_BLOCK_SIZE);
		*(rx_desc_addr+1) = (uint32_t)(EMAC_RX_DESC_INT | (EMAC_BLOCK_SIZE - 1));	/* set size only */    
    }
 
    for ( i = 0; i < EMAC_RX_DESCRIPTOR_COUNT; i++ )
    {
		/* RX status, two words, status info. and status hash CRC. */
		rx_status_addr = (uint32_t *)(RX_STATUS_ADDR + i * 8);	
		*rx_status_addr = (uint32_t)0;	/* initially, set both status info and hash CRC to 0 */
		*(rx_status_addr+1) = (uint32_t)0; 
    }
	
	iowrite32(0x00,net_io_base_addr + rRXCONIN);
}

static void EMAC_set_mac_addr(uint8_t *paddr)
{
	iowrite32((paddr[0] << 8) | paddr[1],net_io_base_addr + rSA0);
	iowrite32((paddr[2] << 8) | paddr[3],net_io_base_addr + rSA1);
	iowrite32((paddr[4] << 8) | paddr[5],net_io_base_addr + rSA2);
}


static int ksz8041_init(void)
{
	uint32_t i;
	uint16_t PHYID;
	uint16_t tempreg;

	iowrite32(ioread32(rPCONP) | 0X40000000,rPCONP);

	/* Set the PIN to RMII */
	i = ioread32(net_io_base_addr + rMAC_MODULEID);

	if (i == OLD_EMAC_MODULE_ID)
		iowrite32 (ioread32(rPINSEL2) | (1)|(1<<2)|(1<<8)|(1<<16)|(1<<18)|(1<<20)|(1<<28)|(1<<30),rPINSEL2);	/* selects P1[0,1,4,8,9,10,14,15] */
	else
		iowrite32 (ioread32(rPINSEL2) | (1)|(1<<2)|(1<<8)|(1<<16)|(1<<18)|(1<<20)|(1<<28)|(1<<30),rPINSEL2);

	iowrite32(ioread32(rPINSEL3) | 0x00000005,rPINSEL3);	/* selects P1[17:16] */



	/* reset : soft,simulation,mcs/rx,rx,mcs/tx,tx */
	iowrite32(0xCF00,net_io_base_addr + rMAC1);  /* [15],[14],[11:8] -> soft resets all MAC internal modules */
	/* RegReset,TxReset,RxReset */
	iowrite32(0x38,net_io_base_addr + rECOMMAND);  	/* reset all datapaths and host registers */

	udelay(10);	/* short delay after reset */

	iowrite32(0x0,net_io_base_addr + rMAC1);		/* deassert all of the above soft resets in MAC1 */

	EMAC_disable_tx();
	EMAC_disable_rx();

	iowrite32(0x0,net_io_base_addr + rMAC2);		/* initialize MAC2 register to default value */

	/* Non back to back inter-packet gap */
	iowrite32(0x12,net_io_base_addr + rIPGR);       /* use the default value recommended in the users manual */


	iowrite32(0x370F,net_io_base_addr + rCLRT); 	/* Use the default value in the users manual */
	iowrite32(0x0600,net_io_base_addr + rMAXF);	    /* Use the default value in the users manual */


	/* host clock divided by 20, no suppress preamble, no scan increment */
	iowrite32(0x18,net_io_base_addr + rMCFG);

	/* RMII configuration */
	iowrite32(0x240,net_io_base_addr + rECOMMAND);

	iowrite32(0x0,net_io_base_addr + rSUPP);

	// probe phy address
	for (i=0;i<32;i++)
	{
		PHYID = PHY_read(i , 2 );
		if (PHYID == 0X0022)
			break;
	}

	if (i >= 32)
	{
		printk("ksz0841 device init fail\n");
		return ERESTARTSYS;
	}

	PHYID = i;
	/// PHY RESET
	PHY_write(PHYID, 0, 0x9200);
	i = 0;
	do
	{
		mdelay(10);
		tempreg = PHY_read(PHYID, 0x1f );
		if (tempreg & 0x001c)
			break;
		if (i++ == 1000)
			break;
	}while (1);

	mdelay(10);
	tempreg = PHY_read(PHYID, 0x1f );
	tempreg &= 0x001c;

	/* INPUT MAC ADDRESS */
	EMAC_set_mac_addr(default_net_mac);

	if (tempreg == 0x0018)//100fdx
	{
		iowrite32(0x31,net_io_base_addr + rMAC2);		/* half duplex, CRC and PAD enabled. */
		iowrite32(ioread32(net_io_base_addr + rSUPP) | 0x0100,net_io_base_addr + rSUPP);		/* RMII Support Reg. speed is set to 100M */
		iowrite32(ioread32(net_io_base_addr + rECOMMAND) | 0x640,net_io_base_addr + rECOMMAND);
		iowrite32(0x15,net_io_base_addr + rIPGT);
	}
	else if (tempreg == 0x0008)//100hdx
	{
		iowrite32(0x30,net_io_base_addr + rMAC2);		/* half duplex, CRC and PAD enabled. */
		iowrite32(ioread32(net_io_base_addr + rSUPP) | 0x0100,net_io_base_addr + rSUPP);		/* RMII Support Reg. speed is set to 100M */
		iowrite32(ioread32(net_io_base_addr + rECOMMAND) | 0x240,net_io_base_addr + rECOMMAND);
		iowrite32(0x12,net_io_base_addr + rIPGT);
	}
	else if (tempreg == 0x0014)//10fdx
	{
		iowrite32(0x31,net_io_base_addr + rMAC2);		/* half duplex, CRC and PAD enabled. */
		iowrite32(0x0,net_io_base_addr + rSUPP);		/* RMII Support Reg. speed is set to 100M */
		iowrite32(ioread32(net_io_base_addr + rECOMMAND) | 0x640,net_io_base_addr + rECOMMAND);
		iowrite32(0x15,net_io_base_addr + rIPGT);
	}
	else if (tempreg == 0x0004)//10hdx
	{
		iowrite32(0x30,net_io_base_addr + rMAC2);		/* half duplex, CRC and PAD enabled. */
		iowrite32( 0x00,net_io_base_addr + rSUPP);		/* RMII Support Reg. speed is set to 100M */
		iowrite32(ioread32(net_io_base_addr + rECOMMAND) | 0x240,net_io_base_addr + rECOMMAND);
		iowrite32(0x12,net_io_base_addr + rIPGT);
	}

	EMAC_tx_descriptor_init();
	EMAC_rx_descriptor_init();

	iowrite32(ioread32(net_io_base_addr + rMAC1) | 0x0002,net_io_base_addr + rMAC1);		/* [1]-Pass All Rx Frame */

	/* Set up RX filter, accept broadcast and perfect station */
	iowrite32( 0x22 | 0x05 | 0x18,net_io_base_addr + rFILTERCTRL);


	iowrite32(0xFFFF,net_io_base_addr + rFINTCLR);	/* clear all MAC interrupts */

	EMAC_enable_rx();
	EMAC_enable_tx();

	iowrite32(0x0c,net_io_base_addr + rEINTEN );	/* Enable all interrupts except SOFTINT and WOL */

	return 0;
}

static int net_set_mac_address(struct net_device *dev, void *addr)
{
	 struct sockaddr *mac_addr = addr;
    if (netif_running(dev))
    {
        return -EBUSY;
    }
    /* Set the device copy of the Ethernet address */
    
    memcpy(dev->dev_addr, mac_addr->sa_data, dev->addr_len);
    
    EMAC_set_mac_addr(dev->dev_addr);
  
    return 0;    
}


static int net_tx(struct sk_buff *skb, struct net_device *dev)
{
	uint8_t *frombuf = (uint8_t*)skb->data;
	uint8_t *tobuf = (uint8_t *)EMAC_TX_BUFFER_ADDR;
	
	unsigned long flag;
	
	uint32_t tx_procduce_index = 0;
	uint32_t tx_consume_index = 0;
	
	uint32_t *tx_desc_addr = 0;
	
	int len = (skb->len < ETH_ZLEN) ? ETH_ZLEN : skb->len;
	int i = 0;
	
	local_irq_save(flag);

#if 0
	for (i = 0; i < skb->len; ++i)
		 *tobuf++ = *frombuf++;
#else
	//memcpy(tobuf,frombuf,skb->len);
#endif	
	
    tx_procduce_index = ioread32(net_io_base_addr + rTXPROIN);
	tx_consume_index = ioread32(net_io_base_addr + rTXCONIN);
	
	if (((tx_procduce_index+1)%4) == tx_consume_index)
		return NETDEV_TX_BUSY;
	
	tobuf += tx_procduce_index * EMAC_BLOCK_SIZE;
	memcpy(tobuf,frombuf,skb->len);
	
	tx_desc_addr = (uint32_t *)(TX_DESCRIPTOR_ADDR + tx_procduce_index * 8);
	*tx_desc_addr = EMAC_TX_BUFFER_ADDR + tx_procduce_index * EMAC_BLOCK_SIZE;
	*(tx_desc_addr+1) = (uint32_t)(EMAC_TX_DESC_INT | EMAC_TX_DESC_LAST | EMAC_TX_DESC_PAD| (len -1));
	

	tx_procduce_index++;
	if (tx_procduce_index == EMAC_TX_DESCRIPTOR_COUNT)
		tx_procduce_index = 0;
	iowrite32(tx_procduce_index,net_io_base_addr + rTXPROIN);
	
	dev->trans_start = jiffies;
	local_irq_restore(flag);
	
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

static void net_rx(struct net_device *dev)
{
	uint32_t rx_produce_index = 0;
	uint32_t rx_consume_index = 0;
	uint32_t *rx_status_addr;
	uint32_t *rx_desc_addr;
	uint8_t *rx_data_addr;
	uint8_t *data;
	
	uint32_t i = 0;
	uint32_t len = 0;
	uint32_t wlen = 0;
	
	rx_produce_index = ioread32(net_io_base_addr + rRXPROIN);
	rx_consume_index = ioread32(net_io_base_addr + rRXCONIN);
	
	if (rx_produce_index == EMAC_RX_DESCRIPTOR_COUNT)
		iowrite32(0x00,net_io_base_addr + rRXPROIN);

	/*have data recv*/
	if (rx_produce_index != rx_consume_index)
	{
		rx_status_addr = (uint32_t *)(RX_STATUS_ADDR + rx_consume_index * 8);
		rx_desc_addr = (uint32_t*)(RX_DESCRIPTOR_ADDR + rx_consume_index * 8);
		rx_data_addr = (uint8_t*)(*rx_desc_addr);
		
		len = (*rx_status_addr & DESC_SIZE_MASK) + 1;
		
		struct sk_buff *skb = netdev_alloc_skb(dev, len + 2);
		
		if (skb)
		{
			skb_reserve(skb, 2);
			data = (uint8_t *)skb_put(skb, len);
			data = skb->data;

#if 0
			for (i = 0; i < len; ++i)
				data[i] = *rx_data_addr++;
#else
			memcpy(data,rx_data_addr,len);
#endif
			skb->dev = dev;
			skb->protocol = eth_type_trans(skb, dev);
			skb->ip_summed = CHECKSUM_UNNECESSARY;
			
			netif_rx(skb);

			dev->last_rx = jiffies;
			
			if ( (*rx_status_addr) & RX_DESC_STATUS_LAST ) 
			{
				if ((rx_consume_index + 1) == EMAC_RX_DESCRIPTOR_COUNT)
					iowrite32(0,net_io_base_addr + rRXCONIN);
				else
					iowrite32(rx_consume_index+1,net_io_base_addr + rRXCONIN);
			}
#if 0			
			rx_consume_index++;
			if (rx_consume_index == EMAC_RX_DESCRIPTOR_COUNT)
				iowrite32(0x00,net_io_base_addr + rRXCONIN);
#endif
		}
			
	}
}

volatile uint32_t g_int_status = 0;
static irqreturn_t ksz8041_isr(int irq,void *devid)
{
	irqreturn_t ret = IRQ_NONE;
	struct net_device *dev = devid;
	
	g_int_status = ioread32(net_io_base_addr + rEINTSTA);
	if (g_int_status)
	{
		iowrite32(0xFF,net_io_base_addr + rFINTCLR);
		if (g_int_status & EMAC_INT_RXDONE)
			net_rx(dev);
		tasklet_schedule(&tasklet);
		ret = IRQ_HANDLED;
	}
	return ret;
}



static void ksz8041_tasklet(unsigned long arg)
{
	struct net_device *dev = (struct net_device*)arg;
	unsigned long  flag;
	
	uint32_t int_status = 0;
	local_irq_save(flag);
	int_status = g_int_status;
	local_irq_restore(flag);
	
	if (int_status & EMAC_INT_RXDONE)
	{
		//net_rx(dev);
    }
	else if (int_status & EMAC_INT_RXOVERRUN)
	{
		EMAC_rx_descriptor_init();
    }
	else if (int_status & EMAC_INT_RXERROR)
	{
		
	}
	else if (int_status & EMAC_INT_RXFINISHED)
	{
		
	}
	else if (int_status & EMAC_INT_TXUNDERRUN)
	{
		
	}
	else if (int_status & EMAC_INT_TXERROR)
	{
		
	}
	else if (int_status & EMAC_INT_TXFINISHED)
	{
		
	}
}





static int net_open(struct net_device *dev)
{
	int iret = -ENOMEM;
	
	/*request memory */
	if (!request_mem_region(NET_BASE_ADDR,NET_MEM_LEN,DEV_NAME))
	   return iret;
	
	   net_io_base_addr = ioremap(NET_BASE_ADDR,NET_MEM_LEN);
	if (!net_io_base_addr)
		goto ioremap_err;
	
	
	/*device init*/
	iret = ksz8041_init();
	if (iret)
		goto request_irq_err;
	
	/*register net irq*/
	iret = request_irq(dev->irq,ksz8041_isr,SA_INTERRUPT | SA_SAMPLE_RANDOM,DEV_NAME,dev);
	if (iret)
	{
		printk("register dev %s irq %d fail\n",dev->name,NET_IRQ);
		goto request_irq_err;
	}
	
	tasklet_init(&tasklet, ksz8041_tasklet, (unsigned long)dev);
	netif_start_queue(dev);
	return iret;
	
request_irq_err:
    iounmap(net_io_base_addr);
ioremap_err:
    release_mem_region(NET_BASE_ADDR,NET_MEM_LEN);
    
	return iret;
}

static int net_release(struct net_device *dev)
{
	netif_stop_queue(dev);
	
	free_irq(NET_IRQ,NULL);
	iounmap(net_io_base_addr);
	release_mem_region(NET_BASE_ADDR,NET_MEM_LEN);
}

static int net_config(struct net_device *dev, struct ifmap *map)
{
	return -EBUSY;
}


static int net_init(struct net_device *dev)
{
	ether_setup(dev);
	
	dev->open = net_open;
	dev->stop = net_release;
	dev->set_config = net_config;
	dev->hard_start_xmit = net_tx;
	dev->set_mac_address = net_set_mac_address;
	dev->flags &= ~(IFF_LOOPBACK | IFF_MULTICAST);
	dev->priv = NULL;
	
	memcpy(dev->dev_addr,default_net_mac,dev->addr_len);
	
	return 0;
}

static struct net_device ksz0841 =
{
	.name = "eth0",
	.init = net_init,
	.base_addr = NET_BASE_ADDR,
	.irq = NET_IRQ,
	.watchdog_timeo = NET_TIMEOUT, 
};

static __init int net_dev_init(void)
{
	int iret = register_netdev(&ksz0841);
	if (iret < 0)
	{
		printk(KERN_ERR"register net device %s error %d\n",ksz0841.name,iret);
	}
	return iret;
}

static __exit int net_dev_exit(void)
{
	unregister_netdev(&ksz0841);
	return 0;
}

module_init(net_dev_init);
module_exit(net_dev_exit);

MODULE_AUTHOR("ygy");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("SmartARM2400 KSZ8041NL net device driver");