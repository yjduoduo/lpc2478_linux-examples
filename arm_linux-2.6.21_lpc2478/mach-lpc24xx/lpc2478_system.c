/*
 * lpc2478 system
 */
#include <asm/hardware.h>
#include <asm/io.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/mm.h>

const char *DEFAULT_IOSCHED = CONFIG_DEFAULT_IOSCHED;
const char *DEFAULT_TCP_CONG = CONFIG_DEFAULT_TCP_CONG;

inline void arch_idle(void)
{
	cpu_do_idle();
}

inline void arch_reset(char mode)
{
	/* machine should reboot */
	mdelay(5000);
	panic("Watchdog timer reset failed!\n");
	printk(" Jump to address 0 \n");
	cpu_reset(0);/*will jump to cpu_arm7_reset in proc-arm67.S,since lpc22xx have no cache,we must modify the code*/
}
