/****************************************Copyright (c)****************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           config.h
** Last modified Date:  2004-09-17
** Last Version:        1.0
** Descriptions:        User Configurable File
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Chenmingji
** Created date:        2004-09-17
** Version: 1.0
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:  		LinEnqiang
** Modified date: 		2007-05-15
** Version: 1.0
** Descriptions: 		For LPC2300
**
*********************************************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE  1
#endif                                                                  /*  TRUE                        */

#ifndef FALSE
#define FALSE 0
#endif                                                                  /*  FALSE                       */

typedef unsigned char  uint8;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  int8;                                            /*  �з���8λ���ͱ���           */
typedef unsigned short uint16;                                          /*  �޷���16λ���ͱ���          */
typedef signed   short int16;                                           /*  �з���16λ���ͱ���          */
typedef unsigned int   uint32;                                          /*  �޷���32λ���ͱ���          */
typedef signed   int   int32;                                           /*  �з���32λ���ͱ���          */
typedef float          fp32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         fp64;                                            /*  ˫���ȸ�������64λ���ȣ�    */

typedef unsigned char  INT8U;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  INT8S;                                           /*  �з���8λ���ͱ���           */
typedef unsigned short INT16U;                                          /*  �޷���16λ���ͱ���          */
typedef signed   short INT16S;                                          /*  �з���16λ���ͱ���          */
typedef unsigned int   INT32U;                                          /*  �޷���32λ���ͱ���          */
typedef signed   int   INT32S;                                          /*  �з���32λ���ͱ���          */
typedef float          FP32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         FP64;                                            /*  ˫���ȸ�������64λ���ȣ�    */

/*********************************************************************************************************
  ARM���������
  ARM specital code
*********************************************************************************************************/
#include    "LPC2400.h"


/*********************************************************************************************************
  �����ӵ�����
  Configuration of the example
*********************************************************************************************************/
//#define	USE_USB				1

#define Fosc                12000000								    /*	OSC = 12MHz                 */

#if USE_USB	
	
	#define Fusbclk	 48000000
	
	#define Fcclk	(Fosc * 4) 										    /* ��Ƶ Fcclk = 48MHz 		    */
	
	/* 
	 * �������USB,��Fcco��Fcclk
	 * ��Fusbclk��275~550M֮���
	 * ��С������				
	 * Fcco��Fcclk��275~550M֮��
	 * ����С����
	 */
	#define Fcco	(Fusbclk * (USBCLKDivValue + 1))							
	#define Fpclk	(Fcclk / 4)
	
	#define PLL_NValue			1
	#define PLL_MValue			(((Fcco / Fosc) * (PLL_NValue + 1) / 2) - 1)
	#define CCLKDivValue		(Fcco / Fcclk - 1)
	#define USBCLKDivValue		5	
#else 
	#define Fcclk	57600000//(Fosc  * 4)											/* ��Ƶ Fcclk = 48MHz		    */
	#define Fcco    (Fcclk * 6)
	
	#define Fpclk	Fcclk//((Fcclk / 2))//(Fcclk / 4)
	
	#define PLL_NValue			9 //1	
	#define PLL_MValue			(((Fcco / Fosc) * (PLL_NValue + 1) / 2) - 1)
	#define CCLKDivValue		(Fcco / Fcclk - 1)
	#define USBCLKDivValue		254		
#endif	                                                                /*  USE_USB                     */

#ifdef __cplusplus
    }
#endif                                                                  /*  __cplusplus                 */
#endif                                                                  /*  __CONFIG_H                  */ 
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
