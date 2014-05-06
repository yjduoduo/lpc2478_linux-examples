/****************************************Copyright (c)****************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           target.c
** Last modified Date:  2004-09-17
** Last Version:        1.0
** Descriptions:        header file of the specific codes for LPC2200 target boards
**                      Every project should include a copy of this file, user may modify it as needed
**--------------------------------------------------------------------------------------------------------
** Created by:          Chenmingji
** Created date:        2004-02-02
** Version:             1.0
** Descriptions:        The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         Chenmingji
** Modified date:       2004-09-17
** Version:             1.01
** Descriptions:        Renewed the template, added more compiler supports
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define IN_TARGET
#define __DEBUG_SDRAM 1
#include "config.h"

/*********************************************************************************************************
* Function name:       DelayNS
* Descriptions:        �������ʱ
* input parameters:    idly	��ʱ������ֵԽ����ʱԽ��
* output parameters:   none
* Returned value:      none
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  sysDelayNS(INT32U  uiDly)
{  
    INT32U  i;
    for(; uiDly > 0; uiDly--){
        for(i = 0; i < 5000; i++);
    }
}

/*********************************************************************************************************
** Function name:           TargetInit
**
** Descriptions:            Initialize the target board; it is called in a necessary place, change it as
**                          needed
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              Chenmingji
** Created Date:            2004/02/02
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void TargetInit (void)
{ 
    
}
/*********************************************************************************************************
** Function name:     SDRAMInit
** Descriptions:      ��ʼ��SDRAM
** input parameters:  none
** output parameters: none
**
** Created by:		  zhangzheng
** Created Date:	  2007.12.10
**-------------------------------------------------------------------------------------------------------
** Modified by:		  zhanghuping		
** Modified date:	  2008.04.25		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  SDRAMInit( void )
{  
    uint32 i, dummy ;
    /* 
    * ʹ��SDRAM�������� 
    */
    PINSEL4    = 0x50000000;
    PINSEL5    = 0x05050555;
    PINSEL6   |= 0x55555555;
    PINSEL8   |= 0x55555555;
    PINSEL9   |= 0x50555555;
    EMCControl = 0x00000001;		                                    /* Disable Address mirror       */
 
    /* 
     * ����SDRAM����ʱ��. 
     */
    EMCDynamictRP   = 1;        
    EMCDynamictRAS  = 3;		
    EMCDynamictSREX = 1;		
    EMCDynamictAPR  = 1;		
    EMCDynamictDAL  = 5;		
    EMCDynamictWR   = 2;		
    EMCDynamictRC   = 3;		
    EMCDynamictRFC  = 3;		
    EMCDynamictXSR  = 5;		
    EMCDynamictRRD  = 1;		
    EMCDynamictMRD  = 2;
    
    EMCDynamicReadConfig =1;		                                    /* Command delayed strategy     */

    /* 
    * Default setting, RAS latency 3 CCLKs, CAS latenty 3 CCLKs. 
    */
    EMCDynamicRASCAS0  = 0x00000303;

  /* 
   * 256MB, 16Mx16, 4 banks, row=13, column=9 
   */
    EMCDynamicConfig0 = 0x00000680;
   
#ifdef __OUT_CHIP                                 
    sysDelayNS(2);
#else   
    sysDelayNS(4);
#endif                                                                  /* __OUT_CHIP                   */
  
    /* 
    * Mem clock enable, CLKOUT runs, send command: NOP 
    */
    EMCDynamicControl = 0x00000183;

#ifdef __OUT_CHIP
    sysDelayNS(2);
#else   
    sysDelayNS(4);
#endif                                                                  /* __OUT_CHIP                   */    
 
    /* 
     * Send command: PRECHARGE-ALL, shortest possible refresh period 
    */
    EMCDynamicControl = 0x00000103;

#ifdef __OUT_CHIP
    sysDelayNS(2);
#else   
    sysDelayNS(4);
#endif                                                                  /* __OUT_CHIP                   */    
 
    /* 
     * set 32 CCLKs between SDRAM refresh cycles 
     */
    EMCDynamicRefresh  = 0x00000002;
    
    for(i = 0; i < 0x80; i++);	                                        /* wait 128 AHB clock cycles    */
    
  /* set 28 x 16CCLKs=448CCLK=7us between SDRAM refresh cycles */
    EMCDynamicRefresh  = 28;
    
    /* 
     * To set mode register in SDRAM, enter mode by issue
     * MODE command, after finishing, bailout and back to NORMAL mode. 
    */   
    EMCDynamicControl = 0x00000083; 
  
    /* 
     * Mem clock enable, CLKOUT runs, send command: MODE 
     */
    dummy=*((volatile uint32 *)(0xa0033000));
    
    EMCDynamicControl = 0x00000000;	                                    /* Send command: NORMAL         */
    EMCDynamicConfig0 |= 0x00080000;	                                /* Enable buffer                */
    sysDelayNS(10);
}


#define   DIVADDVAL   1                                                 
#define   MULVAL      4  

#define  UART_BPS	  115200			// ����ͨѶ������

/****************************************************************************
* ��    �ƣ�UART0_Ini()
* ��    �ܣ���ʼ������0������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ115200
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void  UART0_Init(void)
{  
    //uint16 Fdiv;

    U0LCR = 0x83;						// DLAB = 1�������ò�����
    //Fdiv = (Fpclk / 16) / UART_BPS;		// ���ò�����
    U0DLM = 0;							
    U0DLL = 50;						
    U0LCR = 0x03;
	U0FDR    = (DIVADDVAL << 0)|(MULVAL << 4);   
	PINSEL0  = (PINSEL0 & 0xFFFFFF0F) | (0x01 << 4) | (0x01 << 6);	    /* ����P0.2��P0.3���ӵ�UART0    */
}
				
void UART1_Init(void)
{
	{  
   // uint16 Fdiv;

    U1LCR = 0x83;						// DLAB = 1�������ò�����
    //Fdiv = (Fpclk / 16) / UART_BPS;		// ���ò�����
    U1DLM = 0;							
    U1DLL = 25;						
    U1LCR = 0x03;
	U1FDR    = (DIVADDVAL << 0)|(MULVAL << 4);   
	PINSEL7 = (0x03 << 0) | (0x03 << 2);	                            /* ����P3.16��P3.17�ܽ����ӵ�   */
	                                                                    /* UART1                        */
	
}
}

void UART2_Init(void)
{
	PCONP  |= 1 << 24;		
	U2LCR = 0x83;						// DLAB = 1�������ò�����
    //Fdiv = (Fpclk / 16) / UART_BPS;		// ���ò�����
    U2DLM = 0;							
    U2DLL = 25;						
    U2LCR = 0x03;
	U2FDR    = (DIVADDVAL << 0)|(MULVAL << 4);   
	PINSEL0 = (PINSEL0 & 0xFF0FFFFF) | (0x01 << 20) | (0x01 << 22);	                           
	                                                                    /* UART2                        */
}

void UART3_Init(void)
{
	PCONP  |= 1 << 25;		
	U3LCR = 0x83;						// DLAB = 1�������ò�����
    //Fdiv = (Fpclk / 16) / UART_BPS;		// ���ò�����
    U3DLM = 0;							
    U3DLL = 25;						
    U3LCR = 0x03;
	U3FDR    = (DIVADDVAL << 0)|(MULVAL << 4);   
	PINSEL9 = (PINSEL9 &(~(0x0F << 24))) | (0x03 << 24) | (0x03 << 26); /* ����P4.28��P4.29���ӵ�UART3  */
	                                                                    /* UART2                        */
}

/****************************************************************************
* ��    �ƣ�UART0_SendByte()
* ��    �ܣ��򴮿ڷ����ֽ����ݣ����ȴ�������ϡ�
* ��ڲ�����data		Ҫ���͵�����
* ���ڲ�������
****************************************************************************/
void  UART0_SendByte(uint8 data)
{  
    U0THR = data;				    	// ��������
    
    while( (U0LSR&0x40)==0 );	    	// �ȴ����ݷ������
}

/****************************************************************************
* ��    �ƣ�UART0_SendStr()
* ��    �ܣ��򴮿ڷ���һ�ַ���
* ��ڲ�����srt		Ҫ���͵��ַ�����ָ��
* ���ڲ�������
****************************************************************************/
void  UART0_SendStr(uint8 const *str)
{  
    while(1)
    {  
        if( *str == '\0' ) break;
        UART0_SendByte(*str++);	    	// ��������
    }
}

/*********************************************************************************************************
** Function name:           TargetResetInit
** Descriptions:            Initialize the target
** input parameters:        None
** output parameters:       None
** Returned value:          None
** Created by:              Chenmingji
** Created Date:            2004/02/02
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void TargetResetInit (void)
{
#ifdef __IN_RAM 
    SDRAMInit();                                                        /* �����ⲿSDRAM                */  
    MEMMAP = 0x02;                   									/* remap ��Ƭ��Flash			*/                                                                               
#endif 
#ifdef __IN_CHIP 
    SDRAMInit();                                                        /* �����ⲿSDRAM                */  
    MEMMAP = 0x01;                   									/* remap ��Ƭ��Flash			*/                                                                               
#endif                                                                  /* __IN_CHIP                    */

#ifdef __OUT_CHIP    
     
    MEMMAP           = 0x02;                   							/* remap ���ڲ�RAM	  	        */

	EMCControl       = 0x01;											/* ʹ���ⲿEMC����ֹ����        */
	SDRAMInit();                                                        /* �����ⲿSDRAM                */
#endif                                                                  /* __OUT_CHIP                   */ 		

#ifdef __DEBUG_SDRAM 
    MEMMAP = 0x2;                  	 	                                /* remap ���ڲ�SRAMӳ��         */
#endif                                                                  /* __DEBUG_SDRAM                */

	if ( PLLSTAT & (1 << 25) )
    {
		PLLCON = 1;														/* Enable PLL, disconnected     */
		PLLFEED = 0xAA;
		PLLFEED = 0x55;
    }

    PLLCON = 0;															/* Disable PLL, disconnected    */
    PLLFEED = 0xAA;
    PLLFEED = 0x55;
    while(PLLSTAT & (3 << 24));	
	
    SCS = (SCS & 0x04)|0x20;             								/* Enable the mainOSC,1MHz~20MHz*/
	while((SCS & 0x40) == 0 );			 								/* Wait until main OSC is usable*/
	
	CLKSRCSEL = 0x01;					 								/* select main OSC as the PLL 	*/
																		/* clock source 				*/
										 
    PLLCFG  = (((PLL_NValue) << 16)|(PLL_MValue));    
      
	PLLCON = 1;
    PLLFEED = 0xAA;														/* Enable but disconnect the PLL*/
    PLLFEED = 0x55;	
    while (((PLLSTAT & (1 << 24)) == 0));								/* Wait until the PLL is usable */
	
	CCLKCFG = CCLKDivValue;	
	   	
	#if USE_USB 			
 		USBCLKCFG = USBCLKDivValue;										/* usbclk = 480 MHz/10 = 48 MHz */
 		PCONP |= 0x80000000;											/* Turn On USB PCLK 			*/
	#else 	  		
		USBCLKCFG = USBCLKDivValue;	
		PCONP &= 0x7FFFFFFF;    	
	#endif                                                              /* USE_USB                      */ 		
	while ( ((PLLSTAT & (1 << 26)) == 0) );								/* Check lock bit status 		*/
	while (((PLLSTAT & 0x00007FFF) != PLL_MValue) && (((PLLSTAT & 0x00FF0000) >> 16) != PLL_NValue));
	
 	PLLCON  = 3;														/* connect the PLL 				*/
    PLLFEED = 0xAA;
    PLLFEED = 0x55;	    						
	while (((PLLSTAT & (1 << 25))!= (1 << 25)));						/* Wait until the PLL is 		*/ 
																		/* connected and locked 		*/
	
	/*  
	 *  Set system timers for each component  
	 */
#if (Fpclk / (Fcclk / 4)) == 1

    PCLKSEL0 = 0x00000000;												/* PCLK is 1/4 CCLK 	        */
    PCLKSEL1 = 0x00000000;

#else

    #if (Fpclk / (Fcclk / 4)) == 2

    PCLKSEL0 = 0xAAAAAAAA;												/* PCLK is 1/2 CCLK 			*/
    PCLKSEL1 = 0xAAAAAAAA;	 

    #else

    PCLKSEL0 = 0x55555555;												/* PCLK is the same as CCLK 	*/
    PCLKSEL1 = 0x55555555;	
    
    #endif 
#endif

	/* 
	 *  Set memory accelerater module
	 */     
   MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
    #if Fcclk < 40000000
    	MAMTIM = 2;
    #else
   		MAMTIM = 3;
    #endif
#endif
    MAMCR = 2;      	
   
    /* 
     *  Add your codes here 
     */
    UART0_Init();
	UART1_Init();
	UART2_Init();
	UART3_Init();
	UART0_SendStr("The mini boot have start...\r\n");
	UART0_SendStr("now boot the uclinux kernel...\r\n");
	UART0_SendStr("loading...\r\n");
    return;
}



/*********************************************************************************************************
  End File
*********************************************************************************************************/
