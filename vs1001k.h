/* $Id$ */

/*
 *  (C) Copyright 2003-2005 Wojtek Kaniewski <wojtekka@irc.pl>
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

#ifndef __VS1001K_H
#define __VS1001K_H

#include <avr/io.h>
#include <inttypes.h>

#define vs_reset_set() sbi(PORTB, 1)
#define vs_reset_clear() cbi(PORTB, 1)
#define vs_bsync_set() sbi(PORTB, 4)
#define vs_bsync_clear() cbi(PORTB, 4)
#define vs_cs_set() sbi(PORTB, 3)
#define vs_cs_clear() cbi(PORTB, 3)
#define vs_dreq_get() bit_is_set(PINB, 2)
#define vs_dclk_set() sbi(PORTB, 0)
#define vs_dclk_clear() cbi(PORTB, 0)
//#define vs_dreq_wait() loop_until_bit_is_set(PINB, 2)

#define VS_MODE 0
#define		VS_SM_DIFF _BV(0)
#define		VS_SM_FFWD _BV(1)
#define		VS_SM_RESET _BV(2)
#define		VS_SM_PDOWN _BV(4)
#define		VS_SM_BASS _BV(7)
#define		VS_SM_DACT _BV(8)
#define		VS_SM_BYTEORD _BV(9)
#define		VS_SM_IBMODE _BV(10)
#define		VS_SM_IBCLK _BV(11)
#define VS_STATUS 1
#define		VS_ANALOG_PDOWN _BV(2)
#define VS_CLOCKF 3
#define VS_DECODE_TIME 4
#define VS_AUDATA 5
#define VS_WRAM 6
#define VS_WRAMADDR 7
#define VS_HDAT0 8
#define VS_HDAT1 9
#define VS_AIADDR 10
#define VS_VOL 11
#define VS_AICTRL0 13
#define VS_AICTRL1 14

#define VS_HIGH(x) (((x) >> 8) & 255)
#define VS_LOW(x) ((x) & 255)

#define VS_XTAL 12288000
#define VS_CLOCKF_VALUE (0x8000 | (VS_XTAL / 2000))

extern uint8_t vs_volume;

extern void vs_data_write(uint8_t data);
extern void vs_data_write_buf(uint8_t size);
extern void _vs_command_write(uint8_t command, uint8_t hi, uint8_t lo);
#define vs_command_write(x,y) _vs_command_write((x), VS_HIGH(y), VS_LOW(y))
extern void vs_volume_set(uint8_t volume);
#define vs_volume_mute() vs_volume_set(254)
extern void vs_reset(void);
extern void vs_init(void);
extern void vs_dreq_wait(void);

#endif /* __VS1001K_H */
