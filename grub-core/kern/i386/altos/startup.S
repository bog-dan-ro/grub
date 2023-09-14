/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2001,2002,2003,2005,2006,2007,2008,2009 Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <grub/symbol.h>

	.globl _start
_start:
	cld
	jmp _after_magic

	// add altos magic number
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02

_after_magic:
	movl	$BSS_START_SYMBOL, %edi
	movl	$END_SYMBOL, %ecx
	subl	%edi, %ecx
	xorl	%eax, %eax
	cld
	rep
	stosb
	call EXT_C(grub_main)
