/*
 *  lpc2478 base memory vairable define
 */
#include <linux/compiler.h>
#include <asm/sizes.h>

const unsigned long TASK_SIZE = CONFIG_DRAM_SIZE;
const unsigned long TASK_UNMAPPED_BASE = 0x00000000;

const unsigned long PHYS_OFFSET = CONFIG_DRAM_BASE;

const unsigned long END_MEM = (CONFIG_DRAM_BASE + CONFIG_DRAM_SIZE);

const unsigned long PAGE_OFFSET = CONFIG_DRAM_BASE;

const unsigned long ISA_DMA_THRESHOLD = 0xffffffff;

const unsigned long CONSISTENT_DMA_SIZE = SZ_2M;

const unsigned long VECTORS_BASE = CONFIG_VECTORS_BASE;

void arch_adjust_zones(unsigned long node,unsigned size,unsigned holes)
{
}


unsigned long __virt_to_phys(unsigned long virt_addr)
{
	return virt_addr;
}

unsigned long __phys_to_virt(unsigned long phys_addr)
{
	return phys_addr;
}

unsigned long __virt_to_bus(unsigned long virt_addr)
{
	return virt_addr;
}

unsigned long __bus_to_virt(unsigned long bus_addr)
{
	return bus_addr;
}

void set_vmalloc_start(void)
{
}