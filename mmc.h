/* $Id$ */

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

#ifndef __MMC_H
#define __MMC_H

#include <avr/io.h>
#include <inttypes.h>

#define mmc_cs_set() sbi(PORTD, 5)
#define mmc_cs_clear() cbi(PORTD, 5)

#define MMC_GO_IDLE_STATE 0x40
#define MMC_SEND_OP_COND 0x41
#define MMC_READ_SINGLE_BLOCK 0x51

extern uint8_t mmc_buf[32];
extern uint32_t mmc_addr;

extern void mmc_init(void);
extern void mmc_read_block(uint32_t addr);
extern void mmc_read_block_start(void);
extern void mmc_read_block_raw(void);
extern void mmc_read_block_end(void);

#endif /* __MMC_H */
