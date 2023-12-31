/* console.c - console interface layer for U-Boot platforms */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2013  Free Software Foundation, Inc.
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

#include <grub/term.h>
#include <grub/misc.h>
#include <grub/types.h>
#include <grub/err.h>
#include <grub/terminfo.h>
#include <grub/machine/console.h>

static void
put (struct grub_term_output *term __attribute__ ((unused)), const int c)
{
    asm volatile (
        "movw $0x0102, %%bx\n"
        "int $0x28\n"
    :
    :"al"(c): "bx", "memory", "cc");
}

static int
readkey (struct grub_term_input *term __attribute__ ((unused)))
{
  char key = 0;
  asm volatile (
      "movw $0x0103, %%bx\n"
      "int $0x28\n"
      "movb %%al, %0\n"
      :"=m"(key)
      :: "bx", "memory", "cc");
      
  return key ? key : -1;
}

static void
uboot_console_setcursor (struct grub_term_output *term
			 __attribute__ ((unused)), int on
			 __attribute__ ((unused)))
{
  grub_terminfo_setcursor (term, on);
}

static grub_err_t
uboot_console_init_input (struct grub_term_input *term)
{
  return grub_terminfo_input_init (term);
}


static grub_err_t
uboot_console_init_output (struct grub_term_output *term)
{
  grub_terminfo_output_init (term);

  return 0;
}

struct grub_terminfo_input_state uboot_console_terminfo_input = {
  .readkey = readkey
};

static struct grub_terminfo_output_state grub_console_terminfo_output = {
  .put = put,
  /* FIXME: In rare cases when console isn't serial,
     determine real width.  */
  .size = { 80, 24 }
};

static struct grub_term_input uboot_console_term_input = {
  .name = "console",
  .init = uboot_console_init_input,
  .getkey = grub_terminfo_getkey,
  .data = &uboot_console_terminfo_input
};

static struct grub_term_output uboot_console_term_output = {
  .name = "console",
  .init = uboot_console_init_output,
  .putchar = grub_terminfo_putchar,
  .getwh = grub_terminfo_getwh,
  .getxy = grub_terminfo_getxy,
  .gotoxy = grub_terminfo_gotoxy,
  .cls = grub_terminfo_cls,
  .setcolorstate = grub_terminfo_setcolorstate,
  .setcursor = uboot_console_setcursor,
  .flags = GRUB_TERM_CODE_TYPE_ASCII,
  .data = &grub_console_terminfo_output,
  .progress_update_divisor = GRUB_PROGRESS_FAST
};

static struct grub_term_output grub_console_term_output = {
  .name = "console",
  .init = 0,
  .putchar = grub_terminfo_putchar,
  .getxy = grub_terminfo_getxy,
  .getwh = grub_terminfo_getwh,
  .gotoxy = grub_terminfo_gotoxy,
  .cls = grub_terminfo_cls,
  .refresh = 0,
  .setcolorstate = grub_terminfo_setcolorstate,
  .setcursor = grub_terminfo_setcursor,
  .flags = GRUB_TERM_CODE_TYPE_ASCII,
  .data = &grub_console_terminfo_output,
};

void
grub_altos_console_init (void)
{
  grub_term_register_input ("console", &uboot_console_term_input);
  grub_term_register_output ("console", &uboot_console_term_output);
  grub_terminfo_init ();
  grub_terminfo_output_register (&grub_console_term_output, "vt100-color");
}

// void
// grub_console_init_early (void)
// {
//   grub_term_register_input ("console", &uboot_console_term_input);
//   grub_term_register_output ("console", &uboot_console_term_output);
// }


// /*
//  * grub_console_init_lately():
//  *   Initializes terminfo formatting by registering terminal type.
//  *   Called after heap has been configured.
//  *
//  */
// void
// grub_console_init_lately (void)
// {
//   const char *type;

//   /* See if explicitly set by U-Boot environment */
//   type = grub_uboot_env_get ("grub_term");
//   if (!type)
//     type = "vt100";

//   grub_terminfo_init ();
//   grub_terminfo_output_register (&uboot_console_term_output, type);
// }

// void
// grub_console_fini (void)
// {
// }
