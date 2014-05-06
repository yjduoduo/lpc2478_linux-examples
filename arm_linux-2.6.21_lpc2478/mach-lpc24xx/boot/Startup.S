/****************************************Copyright (c)***************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**--------------File Info--------------------------------------------------------------------------------
** File name: 			Startup.s
** Last modified Date:  
** Last Version: 		
** Descriptions: 		The start up codes for LPC2100, including the initializing codes for the entry 
**                     point of exceptions and the stacks of user tasks. Every project should have a 
**                     independent copy of this file for related modifications
**-------------------------------------------------------------------------------------------------------
** Created by: 		Chenmingji
** Created date:   	2004-02-02
** Version:			1.0
** Descriptions: 		The original version
**-------------------------------------------------------------------------------------------------------
** Modified by: 		LinEnqiang
** Modified date:		2008-05-05	
** Version:			1.01
** Descriptions: 		增加中断管理支持 for LPC2400
**
********************************************************************************************************/
//#include "LPC2400.h"
                                                                      
#include <linux/linkage.h>
#include <linux/init.h>

.equ NoInt ,                       0x80                                    /*  禁止 IRQ 中断宏定义        */
.equ NoFIQ,		        	        0x40                                    /*  禁止 FIQ 中断宏定义        */

.equ USR32Mode           ,         0x10                                    /*  用户模式宏定义             */
.equ SVC32Mode           ,         0x13                                    /*  管理模式宏定义             */
.equ SYS32Mode           ,         0x1f                                    /*  系统模式宏定义             */
.equ IRQ32Mode           ,         0x12                                    /*  IRQ模式宏定义              */
.equ FIQ32Mode           ,         0x11                                    /*  FIQ模式宏定义              */

.equ WAITWEN0             ,        0x0F                                    /* 配置EMCStaticWaitWen0       */
.equ WAITOEN0             ,        0x0F                                    /* 配置EMCStaticWaitOen0       */ 
.equ WAITRD0              ,        0x0F                                    /* 配置EMCStaticWaitRd0        */
.equ WAITPAGE0            ,        0x1F                                    /* 配置EMCStaticWaitPage0      */
.equ WAITWR0              ,        0x0F                                    /* 配置EMCStaticWaitWr0        */
.equ WAITTURN0            ,        0x0F                                    /* 配置EMCStaticWaitTurn0      */

.equ WAITWEN1             ,        0x00                                    /* 配置EMCStaticWaitWen1       */
.equ WAITOEN1             ,        0x01                                    /* 配置EMCStaticWaitOen1       */ 
.equ WAITRD1              ,        0x04                                    /* 配置EMCStaticWaitRd1        */
.equ WAITPAGE1            ,        0x1F                                    /* 配置EMCStaticWaitPage1      */
.equ WAITWR1              ,        0x02                                    /* 配置EMCStaticWaitWr1        */
.equ WAITTURN1            ,        0x0F                                    /* 配置EMCStaticWaitTurn1      */

.equ WAITWEN2            ,         0x0F                                    /* 配置EMCStaticWaitWen2       */
.equ WAITOEN2            ,         0x0F                                    /* 配置EMCStaticWaitOen2       */ 
.equ WAITRD2             ,         0x0F                                    /* 配置EMCStaticWaitRd2        */
.equ WAITPAGE2           ,         0x1F                                    /* 配置EMCStaticWaitPage2      */
.equ WAITWR2             ,         0x0F                                    /* 配置EMCStaticWaitWr2        */
.equ WAITTURN2           ,         0x0F                                    /* 配置EMCStaticWaitTurn2      */

.equ WAITWEN3            ,         0x0F                                    /* 配置EMCStaticWaitWen3       */
.equ WAITOEN3            ,         0x0F                                    /* 配置EMCStaticWaitOen3       */ 
.equ WAITRD3             ,         0x0F                                    /* 配置EMCStaticWaitRd3        */
.equ WAITPAGE3           ,         0x1F                                    /* 配置EMCStaticWaitPage3      */
.equ WAITWR3             ,         0x0F                                    /* 配置EMCStaticWaitWr3        */
.equ WAITTURN3           ,         0x0F                                    /* 配置EMCStaticWaitTurn3      */


.equ BCFG_08DEF,	        0x00000000                              /*   8Bit Bus                  */
.equ BCFG_16DEF,	        0x00000001                              /*  16Bit Bus                  */
.equ BCFG_32DEF,	        0x00000002                              /*  32Bit Bus                  */

/*                       |  页模式    |  片选极性  |字节定位状态|  延长等待  |  写缓冲区  |   写保护   |
                       |    PM      |    PC      |    PB      |     EW     |     B      |     P      |
                       |0:禁能1:使能| 0:低 1:高  |            |0:禁能1:使能|0:禁能1:使能|0:禁能1:使能|  */
.equ vBCFG0,   ((0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20))
.equ vBCFG1,   ((0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20))
.equ vBCFG2,   ((0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20))
.equ vBCFG3,   ((0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20))

.equ STATICCFG0,   ( BCFG_16DEF | vBCFG0 ) 
.equ STATICCFG1,   ( BCFG_16DEF | vBCFG1 )
.equ STATICCFG2,   ( BCFG_16DEF | vBCFG2 )
.equ STATICCFG3,	  ( BCFG_16DEF | vBCFG3 )



        .extern TargetResetInit
		.section	".init.start","ax"
	    .type	stext, #function
ENTRY(stext)                                                             /*  中断向量表                 */
        LDR         PC, ResetAddr
        LDR     	PC, UndefinedAddr
        LDR    	 	PC, SWI_Addr
        LDR     	PC, PrefetchAddr
        LDR     	PC, DataAbortAddr
        .word     	0xb9206e50
        LDR     	PC, [PC, #-0x120]
        LDR     	PC, FIQ_Addr

ResetAddr:           .word     ResetInit
UndefinedAddr:       .word     Undefined
SWI_Addr:            .word     SoftwareInterruptAdd
PrefetchAddr:        .word     PrefetchAbort
DataAbortAddr:       .word     DataAbort
Nouse:               .word     0
IRQ_Addr:            .word     0
FIQ_Addr:            .word     FIQ_Handler

Undefined:
        B       .
PrefetchAbort:
        B       .
DataAbort:
        B       .               
SoftwareInterruptAdd: 
        B       .
FIQ_Handler:
        B       .

#undef EMCStaticConfig0
#undef EMCStaticConfig1
#undef EMCStaticConfig2
#undef EMCStaticConfig3

#define EMCStaticConfig0 (0xffe08000+0x200)
#define EMCStaticConfig1 (0xffe08000+0x220)
#define EMCStaticConfig2 (0xffe08000+0x240)
#define EMCStaticConfig3 (0xffe08000+0x260)
		
/********************************************************************************************************
** Function name:          ResetInit
**
** Descriptions:           复位处理
**
** input parameters:       None
** Returned value:         None
**
** Created by:             Chenmingji
** Created Date:           2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:            LinEnqiang
** Modified date:          2008-01-15
**-------------------------------------------------------------------------------------------------------
********************************************************************************************************/
ResetInit:  
        //BL      RunFirst
              
        LDR     R1, =STATICCFG0		                                    /* 配置EMCStaticConfig0        */
        LDR     R2, =WAITWEN0			                                /* 配置EMCStaticWaitWen0       */
        LDR     R2, =WAITWEN0			                                /* 配置EMCStaticWaitWen0       */
        LDR     R3, =WAITOEN0		                                    /* 配置EMCStaticWaitOen0       */
        LDR     R4, =WAITRD0		    	                            /* 配置EMCStaticWaitRd0        */
        LDR     R5, =WAITPAGE0			                                /* 配置EMCStaticWaitPage0      */
        LDR     R6, =WAITWR0	    		                            /* 配置EMCStaticWaitWr0        */			   
        LDR     R7, =WAITTURN0			                                /* 配置EMCStaticWaitTurn0      */
        LDR		R0, =EMCStaticConfig0 
        STMIA   R0!, {R1-R7}  
 
        LDR     R1, =STATICCFG1		                                    /* 配置EMCStaticConfig1        */
        LDR     R2, =WAITWEN1			                                /* 配置EMCStaticWaitWen1       */
        LDR     R3, =WAITOEN1		                                    /* 配置EMCStaticWaitOen1       */
        LDR     R4, =WAITRD1		    	                            /* 配置EMCStaticWaitRd1        */
        LDR     R5, =WAITPAGE1			                                /* 配置EMCStaticWaitPage1      */
        LDR     R6, =WAITWR1	    		                            /* 配置EMCStaticWaitWr1        */			   
        LDR     R7, =WAITTURN1			                                /* 配置EMCStaticWaitTurn1      */
        LDR		R0, =EMCStaticConfig1
        STMIA   R0!, {R1-R7}
    
        LDR     R1, =STATICCFG2		                                    /* 配置EMCStaticConfig2        */
        LDR     R2, =WAITWEN2			                                /* 配置EMCStaticWaitWen2       */
        LDR     R3, =WAITOEN2		                                    /* 配置EMCStaticWaitOen2       */
        LDR     R4, =WAITRD2		    	                            /* 配置EMCStaticWaitRd2        */
        LDR     R5, =WAITPAGE2			                                /* 配置EMCStaticWaitPage2      */
        LDR     R6, =WAITWR2	    		                            /* 配置EMCStaticWaitWr2        */			   
        LDR     R7, =WAITTURN2			                                /* 配置EMCStaticWaitTurn2      */
        LDR		R0, =EMCStaticConfig2       
        STMIA   R0!, {R1-R7}  
        
        LDR     R1, =STATICCFG3		                                    /* 配置EMCStaticConfig3        */
        LDR     R2, =WAITWEN3			                                /* 配置EMCStaticWaitWen3       */
        LDR     R3, =WAITOEN3		                                    /* 配置EMCStaticWaitOen3       */
        LDR     R4, =WAITRD3		    	                            /* 配置EMCStaticWaitRd3        */
        LDR     R5, =WAITPAGE3			                                /* 配置EMCStaticWaitPage3      */
        LDR     R6, =WAITWR3	    		                            /* 配置EMCStaticWaitWr3        */			   
        LDR     R7, =WAITTURN3			                                /* 配置EMCStaticWaitTurn3      */
        LDR		R0, =EMCStaticConfig3
        STMIA   R0!, {R1-R7}
        
		LDR     SP,=0xA0500000
        BL      TargetResetInit         		                        /*  目标板基本初始化           */       
		
                    		   
#include "../../arch/arm/kernel/head-nommu.S"
       .end
