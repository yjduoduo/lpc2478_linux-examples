/*
 *  linux/include/asm-arm/arch-LPC24xx/irqs.h
 *
 *  Copyright (C) 2004 Philips Semiconductors
 *
 *  IRQ number definition
 *  All IRQ numbers of the LPC24xx CPUs.
 *
 */

#ifndef __LPC24xx_irqs_h
#define __LPC24xx_irqs_h                        1

#define  LPC2478_NR_IRQS     32
//#define NR_IRQS			 32

#define LPC24xx_INTERRUPT_WDINT	  0	/* Watchdog int. 0 */
#define LPC24xx_INTERRUPT_RSV0	  1	/* Reserved int. 1 */
#define LPC24xx_INTERRUPT_DBGRX	  2	/* Embedded ICE DbgCommRx receive */
#define LPC24xx_INTERRUPT_DBGTX	  3	/* Embedded ICE DbgCommRx Transmit*/
#define LPC24xx_INTERRUPT_TIMER0  4	/* Timer 0 */
#define LPC24xx_INTERRUPT_TIMER1  5	/* Timer 1 */
#define LPC24xx_INTERRUPT_UART0	  6	/* UART 0 */
#define LPC24xx_INTERRUPT_UART1   7	/* UART 1 */
#define LPC24xx_INTERRUPT_PWM0 	  8	/* PWM 0 */
#define LPC24xx_INTERRUPT_I2C0 	  9	/* I2C 0 */
#define LPC24xx_INTERRUPT_SPI0 	 10	/* SPI 0 */
#define LPC24xx_INTERRUPT_SPI1 	 11	/* SPI 1 */
#define LPC24xx_INTERRUPT_PLL 	 12	/* PLL */
#define LPC24xx_INTERRUPT_RTC 	 13	/* RTC */
#define LPC24xx_INTERRUPT_EINT0	 14	/* Externel Interrupt 0 */
#define LPC24xx_INTERRUPT_EINT1	 15	/* Externel Interrupt 1 */
#define LPC24xx_INTERRUPT_EINT2	 16	/* Externel Interrupt 2 */
#define LPC24xx_INTERRUPT_EINT3	 17	/* Externel Interrupt 3 */
#define LPC24xx_INTERRUPT_ADC 	 18	/* AD Converter */
#define LPC24xx_INTERRUPT_I2C1   19	/* I2C1 */
#define LPC24xx_INTERRUPT_BOD    20	/* Brown Out detect */
#define LPC24xx_INTERRUPT_ETH    21	/* Ethernet */
#define LPC24xx_INTERRUPT_USB    22	/* USB */
#define LPC24xx_INTERRUPT_CAN    23	/* CAN */
#define LPC24xx_INTERRUPT_MCI    24	/* MMC/SD */
#define LPC24xx_INTERRUPT_DMA    25	/* DMA */
#define LPC24xx_INTERRUPT_TIMER2 26	/* Timer 2 */
#define LPC24xx_INTERRUPT_TIMER3 27	/* Timer 3 */
#define LPC24xx_INTERRUPT_UART2  28	/* UART 2 */
#define LPC24xx_INTERRUPT_UART3  29	/* UART 3 */
#define LPC24xx_INTERRUPT_I2C2   30	/* I2C2 */
#define LPC24xx_INTERRUPT_I2S    31	/* I2S */


#endif /* End of __irqs_h */
