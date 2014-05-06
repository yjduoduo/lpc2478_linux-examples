#
# Makefile for the linux kernel.
# modified for uClinux by Hyok S. Choi, 2004
#

AFLAGS_head.o := -DTEXT_OFFSET=$(TEXT_OFFSET)
# Object file lists.

obj-y		:= compat.o entry-armv.o entry-common.o irq.o \
		   process.o ptrace.o semaphore.o setup.o signal.o sys_arm.o \
		   time.o traps.o

obj-$(CONFIG_ISA_DMA_API)	+= dma.o

obj-$(CONFIG_MODULES)		+= armksyms.o module.o




obj-$(CONFIG_OABI_COMPAT)	+= sys_oabi-compat.o



obj-$(CONFIG_CPU_XSCALE)	+= xscale-cp0.o
obj-$(CONFIG_CPU_XSC3)		+= xscale-cp0.o
obj-$(CONFIG_IWMMXT)		+= iwmmxt.o
AFLAGS_iwmmxt.o			:= -Wa,-mcpu=iwmmxt

ifneq ($(CONFIG_ARCH_EBSA110),y)
  obj-y		+= io.o
endif

head-y			:= head$(MMUEXT).o


extra-y := $(head-y) init_task.o vmlinux.lds
