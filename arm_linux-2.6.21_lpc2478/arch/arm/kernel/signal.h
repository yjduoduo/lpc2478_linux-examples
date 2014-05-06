/*
 *  linux/arch/arm/kernel/signal.h
 *
 *  Copyright (C) 2005 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
extern const unsigned long VECTORS_BASE;
#define KERN_SIGRETURN_CODE	(VECTORS_BASE + 0x00000500)

extern const unsigned long sigreturn_codes[7];
