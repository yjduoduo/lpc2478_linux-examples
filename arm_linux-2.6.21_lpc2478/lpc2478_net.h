#ifndef LPC2478_NET_H
#define LPC2478_NET_H

/* EXTERNAL IO CONFIGURATION */
/* Íâ²¿IOÅäÖÃ¼Ä´æÆ÷ */

#define rPINSEL2        (0xE002C008)        
#define rPINSEL3        (0xE002C00C)        
#define rPINSEL4        (0xE002C010)        
#define rPINSEL5        (0xE002C014)        
#define rPINSEL6        (0xE002C018)        
#define rPINSEL7        (0xE002C01C)        
#define rPINSEL8        (0xE002C020)        

#define rPCONP          (0xE01FC0C4)        

/* ETHERNET REGISTER */
/* MAC REGISTER */
#define rMAC1           (0x0000)        
#define rMAC2           (0x0004)        
#define rIPGT           (0x0008)        
#define rIPGR           (0x000C)        
#define rCLRT           (0x0010)        
#define rMAXF           (0x0014)        
#define rSUPP           (0x0018)        
#define rTEST           (0x001C)        
#define rMCFG           (0x0020)        
#define rMCMD           (0x0024)        
#define rMADR           (0x0028)        
#define rMWTD           (0x002C)        
#define rMRDD           (0x0030)        
#define rMIND           (0x0034)        
#define rSA0            (0x0040)        
#define rSA1            (0x0044)        
#define rSA2            (0x0048)        
/* CONTROL REGISTER */
#define rECOMMAND       (0x0100)        
#define rESTATUS        (0x0104)        
#define rRXDESCR        (0x0108)        
#define rRXSTATU        (0x010C)        
#define rRXDENUM        (0x0110)        
#define rRXPROIN        (0x0114)        
#define rRXCONIN        (0x0118)        
#define rTXDESCR        (0x011C)        
#define rTXSTATU        (0x0120)        
#define rTXDENUM        (0x0124)        
#define rTXPROIN        (0x0128)        
#define rTXCONIN        (0x012C)        
#define rTSV0           (0x0158)        
#define rTSV1           (0x015C)        
#define rRSV            (0x0160)        
#define rFLOWCTRLCNT    (0x0170)        
#define rFLOWCTRLSTA    (0x0174)        
/* RX FILTER REGISTER */
#define rFILTERCTRL     (0x0200)        
#define rFILTERWOLSTA   (0x0204)        
#define rFILTERWOLCLR   (0x0208)        
#define rHASHFILTER1    (0x0210)        
#define rHASHFILTER2    (0x0214)        
/* MODULE CONTROL REGISTER */
#define rEINTSTA        (0x0FE0)        
#define rEINTEN         (0x0FE4)        
#define rFINTCLR        (0x0FE8)        
#define rFINTSET        (0x0FEC)        
#define rEPOWERDOWN     (0x0FF4)        

#define rMAC_MODULEID   (0x0FFC)        

#define OLD_EMAC_MODULE_ID ((0X3902<<16) | 0X2000 )   

#define EMAC_RAM_ADDR		0x7FE00000
#define EMAC_RAM_SIZE		0x00004000

/* The Ethernet RAM is configured as below, the starting of EMAC_DESCRIPTOR_ADDR depends
on the EMAC_DESCRIPTOR_COUNT or the TOTAL_DESCRIPTOR_SIZE, at this point, the 
EMAC_DESCRIPTOR_COUNT for both TX and RX is set to 16:

   EMAC_RAM_ADDR	0x7FE00000
   EMAC_DMA_ADDR	0x7FE00000
   .
   .
   .
   EMAC_DMA_END		EMAC_RAM_ADDR + EMAC_RAM_SIZE - TOTAL_DESCRIPTOR_SIZE 
   TX_DESCRIPTOR_ADDR = EMAC_DESCRIPTOR_ADDR = EMAC_RAM_END(EMAC_RAM_ADDR+EMAC_RAM_SIZE) - TOTAL_DESCRIPTOR 
   TX_STATUS_ADDR = TX_DESCRIPTOR_ADDR + TX_DESCRIPTOR_SIZE
   RX_DESCRIPTOR_ADDR = TX_DESCRIPTOR_ADDR + TX_DESCRIPTOR_SIZE + TX_STATUS_SIZE
   RX_STATUS_ADDR = RX_DESCRIPTOR_ADDR + RX_STATUS_SIZE
   ( RX_STATUS_ADDR + RX_STATUS_SIZE = EMAC_RAM_END )!!!!!
   EMAX_RAM_END	0x7FE04000

   Please note that, the descriptors have to be aligned to the 32 bit boundary!!!
   Below descriptor addresses have been carefully aligned to the 32-bit boundary. 
   If not, the descriptors have to be re-aligned!!! 
 */

#define EMAC_TX_DESCRIPTOR_COUNT	0x0004
#define EMAC_RX_DESCRIPTOR_COUNT	0x0004
#define TX_DESCRIPTOR_SIZE	(EMAC_TX_DESCRIPTOR_COUNT * 8)
#define RX_DESCRIPTOR_SIZE	(EMAC_RX_DESCRIPTOR_COUNT * 8)
#define TX_STATUS_SIZE		(EMAC_TX_DESCRIPTOR_COUNT * 4)
#define RX_STATUS_SIZE		(EMAC_RX_DESCRIPTOR_COUNT * 8)
#define TOTAL_DESCRIPTOR_SIZE	(TX_DESCRIPTOR_SIZE + RX_DESCRIPTOR_SIZE + TX_STATUS_SIZE + RX_STATUS_SIZE)
#define EMAC_DESCRIPTOR_ADDR	(EMAC_RAM_ADDR + EMAC_RAM_SIZE - TOTAL_DESCRIPTOR_SIZE) 

#define TX_DESCRIPTOR_ADDR	EMAC_DESCRIPTOR_ADDR
#define TX_STATUS_ADDR		(EMAC_DESCRIPTOR_ADDR + TX_DESCRIPTOR_SIZE)
#define RX_DESCRIPTOR_ADDR	(TX_STATUS_ADDR + TX_STATUS_SIZE)
#define RX_STATUS_ADDR		(RX_DESCRIPTOR_ADDR + RX_DESCRIPTOR_SIZE)

#define EMAC_DMA_ADDR		EMAC_RAM_ADDR
//#define EMAC_DMA_SIZE		EMAC_RAM_ADDR + EMAC_RAM_END - TOTAL_DESCRIPTOR_SIZE

#define EMAC_BLOCK_SIZE			0x600
#define EMAC_TX_BLOCK_NUM		4	
#define EMAC_RX_BLOCK_NUM		4
#define TOTAL_EMAC_BLOCK_NUM	8

#define EMAC_BUFFER_SIZE	(EMAC_BLOCK_SIZE * (EMAC_TX_BLOCK_NUM + EMAC_RX_BLOCK_NUM ))
#define EMAC_TX_BUFFER_ADDR	EMAC_RAM_ADDR
#define EMAC_RX_BUFFER_ADDR	(EMAC_RAM_ADDR + EMAC_BLOCK_SIZE * EMAC_TX_BLOCK_NUM)
/* EMAC Descriptor TX and RX Control fields */
#define EMAC_TX_DESC_INT		0x80000000
#define EMAC_TX_DESC_LAST		0x40000000
#define EMAC_TX_DESC_CRC		0x20000000
#define EMAC_TX_DESC_PAD		0x10000000
#define EMAC_TX_DESC_HUGE		0x08000000
#define EMAC_TX_DESC_OVERRIDE	0x04000000

#define EMAC_RX_DESC_INT		0x80000000

/* EMAC Descriptor status related definition */
#define TX_DESC_STATUS_ERR		0x80000000
#define TX_DESC_STATUS_NODESC	0x40000000
#define TX_DESC_STATUS_UNDERRUN	0x20000000
#define TX_DESC_STATUS_LCOL		0x10000000
#define TX_DESC_STATUS_ECOL		0x08000000
#define TX_DESC_STATUS_EDEFER	0x04000000
#define TX_DESC_STATUS_DEFER	0x02000000
#define TX_DESC_STATUS_COLCNT	0x01E00000	/* four bits, it's a mask, not exact count */

#define RX_DESC_STATUS_ERR		0x80000000
#define RX_DESC_STATUS_LAST		0x40000000
#define RX_DESC_STATUS_NODESC	0x20000000
#define RX_DESC_STATUS_OVERRUN	0x10000000
#define RX_DESC_STATUS_ALGNERR	0x08000000
#define RX_DESC_STATUS_RNGERR	0x04000000
#define RX_DESC_STATUS_LENERR	0x02000000
#define RX_DESC_STATUS_SYMERR	0x01000000
#define RX_DESC_STATUS_CRCERR	0x00800000
#define RX_DESC_STATUS_BCAST	0x00400000
#define RX_DESC_STATUS_MCAST	0x00200000
#define RX_DESC_STATUS_FAILFLT	0x00100000
#define RX_DESC_STATUS_VLAN		0x00080000
#define RX_DESC_STATUS_CTLFRAM	0x00040000

#define DESC_SIZE_MASK			0x000007FF	/* 11 bits for both TX and RX */

/* EMAC interrupt controller related definition */
#define EMAC_INT_RXOVERRUN	0x01 << 0
#define EMAC_INT_RXERROR	0x01 << 1 
#define EMAC_INT_RXFINISHED	0x01 << 2
#define EMAC_INT_RXDONE		0x01 << 3 
#define EMAC_INT_TXUNDERRUN	0x01 << 4
#define EMAC_INT_TXERROR	0x01 << 5 
#define EMAC_INT_TXFINISHED	0x01 << 6
#define EMAC_INT_TXDONE		0x01 << 7 
#define EMAC_INT_SOFTINT	0x01 << 12
#define EMAC_INT_WOL		0x01 << 13 


#endif



#if 0
#define     MAC_MAC1                (0x000) 
#define     MAC_MAC2                (0x004) 
#define     MAC_IPGT                (0x008) 
#define     MAC_IPGR                (0x00C)
#define     MAC_CLRT                (0x010)
#define     MAC_MAXF                (0x014)
#define     MAC_SUPP                (0x018)
#define     MAC_TEST                (0x01C)
#define     MAC_MCFG                (0x020)
#define     MAC_MCMD                (0x024)
#define     MAC_MADR                (0x028)
#define     MAC_MWTD                (0x02C)
#define     MAC_MRDD                (0x030)
#define     MAC_MIND                (0x034)

#define     MAC_SA0                 (0x040) 
#define     MAC_SA1                 (0x044) 
#define     MAC_SA2                 (0x048) 

#define     MAC_COMMAND             (0x100) 
#define     MAC_STATUS              (0x104) 
#define     MAC_RXDESCRIPTOR        (0x108) 
#define     MAC_RXSTATUS            (0x10C) 
#define     MAC_RXDESCRIPTORNUM     (0x110) 
#define     MAC_RXPRODUCEINDEX      (0x114) 
#define     MAC_RXCONSUMEINDEX      (0x118) 
#define     MAC_TXDESCRIPTOR        (0x11C) 
#define     MAC_TXSTATUS            (0x120) 
#define     MAC_TXDESCRIPTORNUM     (0x124) 
#define     MAC_TXPRODUCEINDEX      (0x128) 
#define     MAC_TXCONSUMEINDEX      (0x12C) 

#define     MAC_TSV0                (0x158) 
#define     MAC_TSV1                (0x15C) 
#define     MAC_RSV                 (0x160) 

#define     MAC_FLOWCONTROLCNT      (0x170) 
#define     MAC_FLOWCONTROLSTS      (0x174) 

#define     MAC_RXFILTERCTRL        (0x200)
#define     MAC_RXFILTERWOLSTS      (0x204)
#define     MAC_RXFILTERWOLCLR      (0x208)

#define     MAC_HASHFILTERL         (0x210))
#define     MAC_HASHFILTERH         (0x214))

#define     MAC_INTSTATUS           (0xFE0)
#define     MAC_INTENABLE           (0xFE4)
#define     MAC_INTCLEAR            (0xFE8)
#define     MAC_INTSET              (0xFEC)

#define     MAC_POWERDOWN           (0xFF4)
#define     MAC_MODULEID            (0xFFC)

#endif