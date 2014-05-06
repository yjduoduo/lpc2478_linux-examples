#
# Makefile for the Linux filesystems.
#
# 14 Sep 2000, Christoph Hellwig <hch@infradead.org>
# Rewritten to use lists instead of if-statements.
# 

obj-y :=	open.o read_write.o file_table.o super.o \
		char_dev.o stat.o exec.o pipe.o namei.o fcntl.o \
		ioctl.o readdir.o select.o fifo.o locks.o dcache.o inode.o \
		attr.o bad_inode.o file.o filesystems.o namespace.o aio.o \
		seq_file.o xattr.o libfs.o fs-writeback.o \
		pnode.o drop_caches.o splice.o sync.o utimes.o \
		stack.o

ifeq ($(CONFIG_BLOCK),y)
obj-y +=	buffer.o bio.o block_dev.o direct-io.o mpage.o ioprio.o
else
obj-y +=	no-block.o
endif

obj-$(CONFIG_INOTIFY)		+= inotify.o

obj-$(CONFIG_EPOLL)		+= eventpoll.o


# binfmt_script is always there
obj-y				+= binfmt_script.o


obj-$(CONFIG_BINFMT_FLAT)	+= binfmt_flat.o

obj-$(CONFIG_FS_MBCACHE)	+= mbcache.o




obj-$(CONFIG_DNOTIFY)		+= dnotify.o

obj-$(CONFIG_PROC_FS)		+= proc/
obj-y				+= partitions/
obj-$(CONFIG_SYSFS)		+= sysfs/




 

obj-$(CONFIG_EXT2_FS)		+= ext2/
obj-$(CONFIG_CRAMFS)		+= cramfs/

obj-$(CONFIG_RAMFS)		+= ramfs/

obj-$(CONFIG_FAT_FS)		+= fat/
obj-$(CONFIG_MSDOS_FS)		+= msdos/
obj-$(CONFIG_VFAT_FS)		+= vfat/

obj-$(CONFIG_NFS_FS)		+= nfs/


obj-$(CONFIG_LOCKD)		+= lockd/
obj-$(CONFIG_NLS)		+= nls/




obj-$(CONFIG_NTFS_FS)		+= ntfs/

obj-$(CONFIG_JFFS2_FS)		+= jffs2/

obj-$(CONFIG_ROMFS_FS)		+= romfs/

