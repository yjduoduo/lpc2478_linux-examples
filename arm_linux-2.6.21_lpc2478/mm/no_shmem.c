/*
 *mm/no_shmem.c
 */

#include <linux/mm.h>


inline int shmem_lock(struct file *file, int lock,
struct user_struct *user)
{
	return 0;
}

inline int shmem_set_policy(struct vm_area_struct *vma,
struct mempolicy *new)
{
	return 0;
}

inline struct mempolicy *shmem_get_policy(struct vm_area_struct *vma,
												 unsigned long addr)
{
	return NULL;
}