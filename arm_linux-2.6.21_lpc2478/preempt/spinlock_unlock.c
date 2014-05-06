/*
 * preempt_unlock.c
 * preempte unlock 
 */
#include <linux/spinlock.h>
#include <linux/module.h>

#if defined(CONFIG_DEBUG_SPINLOCK) || defined(CONFIG_PREEMPT) || \
	!defined(CONFIG_SMP)

 void spin_unlock(spinlock_t *lock)
{
	_spin_unlock(lock);
}

 void read_unlock(spinlock_t *lock)
{
	_read_unlock(lock);
}

 void write_unlock(spinlock_t *lock)
{
	_write_unlock(lock);
}

 void spin_unlock_irq(spinlock_t *lock)
{
	_spin_unlock_irq(lock);
}

 void read_unlock_irq(spinlock_t *lock)
{
	_read_unlock_irq(lock);
}

 void write_unlock_irq(spinlock_t *lock)
{
	_write_unlock_irq(lock);
}


#else

 void spin_unlock(spinlock_t *lock)
{
	__raw_spin_unlock(&(lock)->raw_lock); 
	__release(lock);
}

 void read_unlock(spinlock_t *lock)
{
	__raw_read_unlock(&(lock)->raw_lock); 
	__release(lock);
}

 void write_unlock(spinlock_t *lock)
{
	__raw_write_unlock(&(lock)->raw_lock); 
	__release(lock);
}

 void spin_unlock_irq(spinlock_t *lock)
{
	__raw_spin_unlock(&(lock)->raw_lock);	
	__release(lock);			
	local_irq_enable();	
}

 void read_unlock_irq(spinlock_t *lock)
{
	__raw_read_unlock(&(lock)->raw_lock);	
	__release(lock);			
	local_irq_enable();	
}

 void write_unlock_irq(spinlock_t *lock)
{
	__raw_write_unlock(&(lock)->raw_lock);	
	__release(lock);			
	local_irq_enable();	
}

#endif

EXPORT_SYMBOL(spin_unlock);
EXPORT_SYMBOL(read_unlock);
EXPORT_SYMBOL(write_unlock);
EXPORT_SYMBOL(spin_unlock_irq);
EXPORT_SYMBOL(read_unlock_irq);
EXPORT_SYMBOL(write_unlock_irq);