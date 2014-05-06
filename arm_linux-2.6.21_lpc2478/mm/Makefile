#
# Makefile for the linux memory manager.
#

mmu-y			:= nommu.o
mmu-$(CONFIG_MMU)	:= fremap.o highmem.o madvise.o memory.o mincore.o \
			   mlock.o mmap.o mprotect.o mremap.o msync.o rmap.o \
			   vmalloc.o

obj-y			:= bootmem.o filemap.o mempool.o oom_kill.o fadvise.o \
			   page_alloc.o page-writeback.o pdflush.o \
			   readahead.o swap.o truncate.o vmscan.o \
			   prio_tree.o util.o mmzone.o vmstat.o backing-dev.o \
			   $(mmu-y)

ifeq ($(CONFIG_MMU)$(CONFIG_BLOCK),yy)
obj-y			+= bounce.o
endif


obj-$(CONFIG_SHMEM) += shmem.o

obj-$(CONFIG_TINY_SHMEM) += tiny-shmem.o

obj-$(CONFIG_SLAB) += slab.o


