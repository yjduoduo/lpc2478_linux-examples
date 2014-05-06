/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 2.6.21-uc0
 * Thu Aug 20 09:03:05 2009
 */
#define AUTOCONF_INCLUDED

/*kernel*/
#define CONFIG_FLATMEM 1
#define CONFIG_BASE_SMALL 0
#define CONFIG_BROKEN_ON_SMP 1
#define CONFIG_RT_MUTEXES 1
#define CONFIG_RWSEM_GENERIC_SPINLOCK 1
#define CONFIG_FUTEX 1



/*mm*/
#define CONFIG_ZONE_DMA_FLAG 1
#define CONFIG_SLAB 1
#define CONFIG_FLAT_NODE_MEM_MAP 1

#define CONFIG_TINY_SHMEM 1
/*#define CONFIG_SHMEM 1*/
/*#define CONFIG_MMU 1*/



/*module*/
/*#define CONFIG_MODULES 1*/
/*#define CONFIG_MODULE_UNLOAD 1*/


/*float*/
/*#define CONFIG_FPE_NWFPE 1*/
/*#define CONFIG_VFP 1*/          /*not suport by arm7*/


/*fs*/
#define CONFIG_INOTIFY  1
#define CONFIG_EPOLL 1
#define CONFIG_DNOTIFY 1
#define CONFIG_PROC_FS 1
#define CONFIG_SYSFS 1
#define CONFIG_RAMFS 1

/*#define  CONFIG_FS_MBCACHE 1         x*/

#define CONFIG_BINFMT_FLAT 1
#define CONFIG_BINFMT_SHARED_FLAT 1
/*#define CONFIG_BINFMT_ELF 1*/

#define CONFIG_EXT2_FS 1

/*#define CONFIG_CRAMFS 1*/

/*#define CONFIG_FAT_FS 1*/
/*#define CONFIG_MSDOS_FS 1*/
/*#define CONFIG_VFAT_FS 1*/
/*#define CONFIG_NTFS_FS*/
/*#define CONFIG_NLS */   /*need by FAT/MSDOS/VFAT/NTFS*/

/*#define CONFIG_NFS_FS 1*/
/*#define CONFIG_LOCKD 1*/

/*#define CONFIG_JFFS2_FS 1*/

/*#define CONFIG_ROMFS_FS*/


/*irq*/
#define CONFIG_TRACE_IRQFLAGS_SUPPORT 1
#define CONFIG_GENERIC_IRQ_PROBE 1
#define CONFIG_HARDIRQS_SW_RESEND 1
#define CONFIG_GENERIC_HARDIRQS 1

/*system log buf */
#define CONFIG_LOG_BUF_SHIFT 14

/*.config*/
#define CONFIG_DEFCONFIG_LIST "/lib/modules/$UNAME_RELEASE/.config"

/*system*/
#define CONFIG_ZBOOT_ROM_BSS 0x0
#define CONFIG_ZBOOT_ROM_TEXT 0x0
#define CONFIG_EMBEDDED 1
#define CONFIG_SYS_SUPPORTS_APM_EMULATION 1
#define CONFIG_UID16 1
#define CONFIG_SPLIT_PTLOCK_CPUS 4096
#define CONFIG_LOCALVERSION ""
#define CONFIG_UNAME_RELEASE "2.4.20-8"
#define CONFIG_PRINTK 1
#define CONFIG_ARM 1
#define CONFIG_KERNELVERSION "2.6.21-uc0"
#define CONFIG_AEABI   1
#define CONFIG_OABI_COMPAT 1
#define CONFIG_ZONE_DMA 1

/*#define CONFIG_PREEMPT 1*/
/*#define CONFIG_PM 1*/
/*#define CONFIG_PM_LEGACY 1*/


/*ipc*/
#define CONFIG_SYSVIPC 1

/*compiler*/
#define CONFIG_CC_OPTIMIZE_FOR_SIZE 1

/*lib*/
#define CONFIG_CRC32 1
#define CONFIG_GENERIC_HWEIGHT 1
#define CONFIG_ZLIB_DEFLATE 1
#define CONFIG_PLIST 1
#define CONFIG_ZLIB_INFLATE 1


/*block*/
#define CONFIG_BLOCK 1

#define CONFIG_DEFAULT_IOSCHED "noop"
#define CONFIG_IOSCHED_NOOP 1
/*#define  CONFIG_IOSCHED_AS 1*/
/*#define  CONFIG_IOSCHED_DEADLINE 1*/
/*#define  CONFIG_IOSCHED_CFQ 1*/

/*net*/

#define CONFIG_NET 1
#define CONFIG_INET 1
#define CONFIG_TCP_CONG_CUBIC 1
#define CONFIG_IP_FIB_HASH 1
#define CONFIG_NET_ETHERNET 1
#define CONFIG_DEFAULT_TCP_CONG "cubic"
#define CONFIG_NETDEVICES 1
/*#define CONFIG_UNIX 1
#define CONFIG_SUNRPC 1

#define CONFIG_IEEE80211 1  wifi
#define CONFIG_IRDA 1       irda
#define CONFIG_BT 1         bluetooth     
*/


/*time*/
#define CONFIG_HZ 100

/*arch*/
#define CONFIG_CPU_CACHE_V4 1
#define CONFIG_CPU_ARM7TDMI 1
#define CONFIG_CPU_32v4T 1
#define CONFIG_CPU_32 1
#define CONFIG_CPU_ABRT_LV4T 1

/*#define CONFIG_ISA_DMA_API 1*/

/*init*/
#define CONFIG_INIT_ENV_ARG_LIMIT 32
#define CONFIG_GENERIC_CALIBRATE_DELAY 1
#define CONFIG_BLK_DEV_INITRD 1
#define CONFIG_CMDLINE "root=/dev/ram0 initrd=0xA0400000,2000K  rw console=ttyS0,57600N8 ramdisk_size=2000 kgdbwait"// kgdboc=ttyS0,115200"


/*driver*/
/*serial*/
#define CONFIG_SERIAL_CORE 1
#define CONFIG_SERIAL_8250_RUNTIME_UARTS 4
#define CONFIG_SERIAL_8250_CONSOLE 1
#define CONFIG_SERIAL_CORE_CONSOLE 1
#define CONFIG_SERIAL_8250_NR_UARTS 4
#define CONFIG_SERIAL_8250 1

/***block ram*/
#define CONFIG_BLK_DEV_RAM 1
#define CONFIG_BLK_DEV_RAM_COUNT 2
#define CONFIG_BLK_DEV_RAM_BLOCKSIZE 1024
#define CONFIG_BLK_DEV_RAM_SIZE 4096



/*arch memory*/
#define CONFIG_FLASH_SIZE 0x00200000
#define CONFIG_FLASH_MEM_BASE 0x81000000
#define CONFIG_DRAM_BASE 0xA0000000
#define CONFIG_VECTORS_BASE 0x40000000
#define CONFIG_DRAM_SIZE 0x02000000

/*arch system */
#define CONFIG_ARM_CLK  57600000//92160000//48000000
#define CONFIG_LPC24xx_Fosc 12000000
#define CONFIG_MACH_LPC22XX 1
#define CONFIG_ARCH_LPC22XX 1
#define CONFIG_PROCESSOR_ID 0x24680000
#define CONFIG_TLS_REG_EMUL 1
