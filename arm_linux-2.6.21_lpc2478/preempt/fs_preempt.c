/*
 * fs_preempt.c
 * fs preempt
 */
#include <linux/fs.h>
#include <linux/module.h>

/*
* NOTE: in a 32bit arch with a preemptable kernel and
* an UP compile the i_size_read/write must be atomic
* with respect to the local cpu (unlike with preempt disabled),
* but they don't need to be atomic with respect to other cpus like in
* true SMP (so they need either to either locally disable irq around
* the read or for example on x86 they can be still implemented as a
* cmpxchg8b without the need of the lock prefix). For SMP compiles
* and 64bit archs it makes no difference if preempt is enabled or not.
*/
inline loff_t i_size_read(const struct inode *inode)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	loff_t i_size;
	unsigned int seq;

	do {
		seq = read_seqcount_begin(&inode->i_size_seqcount);
		i_size = inode->i_size;
	} while (read_seqcount_retry(&inode->i_size_seqcount, seq));
	return i_size;
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
	loff_t i_size;

	preempt_disable();
	i_size = inode->i_size;
	preempt_enable();
	return i_size;
#else
	return inode->i_size;
#endif
}

EXPORT_SYMBOL(i_size_read);

/*
* NOTE: unlike i_size_read(), i_size_write() does need locking around it
* (normally i_mutex), otherwise on 32bit/SMP an update of i_size_seqcount
* can be lost, resulting in subsequent i_size_read() calls spinning forever.
*/
inline void i_size_write(struct inode *inode, loff_t i_size)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	write_seqcount_begin(&inode->i_size_seqcount);
	inode->i_size = i_size;
	write_seqcount_end(&inode->i_size_seqcount);
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
	preempt_disable();
	inode->i_size = i_size;
	preempt_enable();
#else
	inode->i_size = i_size;
#endif
}
EXPORT_SYMBOL(i_size_write);
