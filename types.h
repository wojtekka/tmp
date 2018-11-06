/* $Id: types.h 11 2005-06-15 16:29:51Z wojtekka $ */

/*
 *  (C) Copyright 2003 Wojtek Kaniewski <wojtekka@irc.pl>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License Version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __TYPES_H
#define __TYPES_H

#include <inttypes.h>

#define byte(ptr,ofs) *(((uint8_t*)&(ptr))+(ofs))

/* idea from yampp. gcc optimizes unions better than logical ands and shifts. */

union dword {
	uint32_t dword;
	uint8_t byte[4];
	uint16_t word[2];
};

union word {
	uint16_t word;
	uint8_t byte[2];
};

/* first two bytes are never read, because we start at 32-byte boundary */

struct partition {
/*	uint8_t state;		   0x1be */
/*	uint8_t start_h;	   0x1bf */
	uint16_t start_cs;	/* 0x1c0 */
	uint8_t type;		/* 0x1c2 */
	uint8_t end_h;		/* 0x1c3 */
	uint16_t end_cs;	/* 0x1c4 */
	uint32_t start;		/* 0x1c6 */
	uint32_t length;	/* 0x1ca */
};

#endif /* __TYPES_H */
