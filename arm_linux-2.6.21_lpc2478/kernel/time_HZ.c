/*
 *deal with time HZ config
 */
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/jiffies.h>
#include <linux/clocksource.h>

unsigned int SHIFT_HZ;
unsigned int SEC_JIFFIE_SC;

void __init get_shift_hz(void)
{
	if ((HZ >= 12) && (HZ < 24))
		SHIFT_HZ = 4;
	else if ((HZ >= 24) && (HZ < 48))
		SHIFT_HZ = 5;
	else if ((HZ >= 48) && (HZ < 96))
		SHIFT_HZ = 6;
	else if ((HZ >= 96) && (HZ < 192))
		SHIFT_HZ = 7;
	else if ((HZ >= 192) && (HZ < 384))
		SHIFT_HZ = 8;
	else if ((HZ >= 384) && (HZ < 768))
		SHIFT_HZ = 9;
	else if ((HZ >= 768) && (HZ < 1536))
		SHIFT_HZ = 10;
	else
	{
		SHIFT_HZ = 0;
	}
}

extern int printk_ratelimit_jiffies ;
extern struct clocksource clocksource_jiffies;


/* The Jiffies based clocksource is the lowest common
 * denominator clock source which should function on
 * all systems. It has the same coarse resolution as
 * the timer interrupt frequency HZ and it suffers
 * inaccuracies caused by missed or lost timer
 * interrupts and the inability for the timer
 * interrupt hardware to accuratly tick at the
 * requested HZ value. It is also not reccomended
 * for "tick-less" systems.
 */
#define NSEC_PER_JIFFY	((u32)(div_u64((((u64)NSEC_PER_SEC)<<8),ACTHZ)))

/* Since jiffies uses a simple NSEC_PER_JIFFY multiplier
 * conversion, the .shift value could be zero. However
 * this would make NTP adjustments impossible as they are
 * in units of 1/2^.shift. Thus we use JIFFIES_SHIFT to
 * shift both the nominator and denominator the same
 * amount, and give ntp adjustments in units of 1/2^8
 *
 * The value 8 is somewhat carefully chosen, as anything
 * larger can result in overflows. NSEC_PER_JIFFY grows as
 * HZ shrinks, so values greater then 8 overflow 32bits when
 * HZ=100.
 */
#define JIFFIES_SHIFT	8

extern u64 jiffies_64;
void __init get_HZ_param(void)
{
	printk_ratelimit_jiffies = 5 * HZ;
	
	init_task.time_slice = HZ;
	
	get_shift_hz();
	SEC_JIFFIE_SC = (31 - SHIFT_HZ);
	if (!((((NSEC_PER_SEC << 2) / TICK_NSEC) << (SEC_JIFFIE_SC - 2)) & 0x80000000))
		SEC_JIFFIE_SC  = (32 - SHIFT_HZ);
	
	clocksource_jiffies.mult = NSEC_PER_JIFFY << JIFFIES_SHIFT;

	jiffies_64 = INITIAL_JIFFIES;
}