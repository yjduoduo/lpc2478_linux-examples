/*
 *  linux/include/asm-arm/arch-lpc22xx/io.h
 *
 * Copyright (C) 2004 Philips Semiconductors
 *
 */
#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H



/*
 * These macros were copied from arch/armnommu/io.h and are used instead
 * of the definitions found there, because we want to do 16/32 bit i/o
 * without byte swapping.
 * --the
 */

#undef __io

#ifndef __iob
  #define __iob(a) __io(a)
#endif

#define outb(v,p)       __raw_writeb(v, p)
#define outw(v,p)       __raw_writew(v, p)
#define outl(v,p)       __raw_writel(v, p)

#define inb(p)          ({ unsigned int __v = __raw_readb(p); __v; })
#define inw(p)          ({ unsigned int __v = __raw_readw(p); __v; })
#define inl(p)          ({ unsigned int __v = __raw_readl(p); __v; })

#define outsb(p,d,l)    __raw_writesb(p, d, l)
#define outsw(p,d,l)    __raw_writesw(p, d, l)
#define outsl(p,d,l)    __raw_writesl(p, d, l)

#define insb(p,d,l)     __raw_readsb(p, d, l)
#define insw(p,d,l)     __raw_readsw(p, d, l)
#define insl(p,d,l)     __raw_readsl(p, d, l)

/*
 * Validate the pci memory address for ioremap.
 */
#define iomem_valid_addr(iomem,size)	(1)


/*
 * Convert PCI memory space to a CPU physical address
 */
#define iomem_to_phys(iomem)	(iomem)

#endif
