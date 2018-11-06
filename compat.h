/* $Id$ */

/*
 *  (C) Copyright 2006 Wojtek Kaniewski <wojtekka@irc.pl>
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

#ifndef __COMPAT_H
#define __COMPATI_H

#ifndef sbi
#define sbi(a,b) do { a |= _BV(b); } while(0)
#endif

#ifndef cbi
#define cbi(a,b) do { a &= ~_BV(b); } while(0)
#endif

#endif /* __COMPAT_H */
