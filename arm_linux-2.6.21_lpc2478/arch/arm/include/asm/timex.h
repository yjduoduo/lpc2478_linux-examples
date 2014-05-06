/*
 *  linux/include/asm-arm/timex.h
 *
 *  Copyright (C) 1997,1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Architecture Specific TIME specifications
 */
#ifndef _ASMARM_TIMEX_H
#define _ASMARM_TIMEX_H


typedef unsigned long cycles_t;

extern const unsigned long CLOCK_TICK_RATE;

static inline cycles_t get_cycles (void)
{
	return 0;
}

#endif
