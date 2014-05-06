/*
 *  lpc2478 base irq config
 */
#include <linux/types.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <asm/arch/irqs.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <asm/arch/serial.h>

const unsigned NR_IRQS = LPC2478_NR_IRQS;

const unsigned int NO_IRQ = ((unsigned int)(-1));

struct irq_desc irq_desc[LPC2478_NR_IRQS] __cacheline_aligned = {
	[0 ... LPC2478_NR_IRQS-1] = {
		.status = IRQ_DISABLED,
		.chip = &no_irq_chip,
		.handle_irq = handle_bad_irq,
		.depth = 1,
		.lock = __SPIN_LOCK_UNLOCKED(irq_desc->lock),
#ifdef CONFIG_SMP
		.affinity = CPU_MASK_ALL
#endif
	}
};

#ifdef CONFIG_HARDIRQS_SW_RESEND
DECLARE_BITMAP(irqs_resend, LPC2478_NR_IRQS);
#endif

#ifdef CONFIG_SERIAL_8250
struct irq_info irq_lists[LPC2478_NR_IRQS];
#endif

struct timer_rand_state *irq_timer_state[LPC2478_NR_IRQS];

unsigned irq_canonicalize(unsigned irq)
{
	return irq;
}

