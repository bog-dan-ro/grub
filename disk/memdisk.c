/* memdisk.c - Access embedded memory disk.  */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2007  Free Software Foundation, Inc.
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

#include <grub/disk.h>
#include <grub/dl.h>
#include <grub/kernel.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/types.h>

static grub_addr_t memdisk_addr;

static int
grub_memdisk_iterate (int (*hook) (const char *name))
{
  return hook ("memdisk");
}

static grub_err_t
grub_memdisk_open (const char *name, grub_disk_t disk)
{
  if (grub_strcmp (name, "memdisk"))
      return grub_error (GRUB_ERR_UNKNOWN_DEVICE, "not a memdisk");

  disk->total_sectors = grub_arch_memdisk_size () / GRUB_DISK_SECTOR_SIZE;
  disk->id = (int) 'mdsk';
  disk->has_partitions = 0;

  return GRUB_ERR_NONE;
}

static void
grub_memdisk_close (grub_disk_t disk __attribute((unused)))
{
}

static grub_err_t
grub_memdisk_read (grub_disk_t disk __attribute((unused)), grub_disk_addr_t sector,
		    grub_size_t size, char *buf)
{
  grub_memcpy (buf, memdisk_addr + (sector << GRUB_DISK_SECTOR_BITS), size << GRUB_DISK_SECTOR_BITS);
  return 0;
}

static grub_err_t
grub_memdisk_write (grub_disk_t disk __attribute((unused)), grub_disk_addr_t sector,
		     grub_size_t size, const char *buf)
{
  grub_memcpy (memdisk_addr + (sector << GRUB_DISK_SECTOR_BITS), buf, size << GRUB_DISK_SECTOR_BITS);
  return 0;
}

static struct grub_disk_dev grub_memdisk_dev =
  {
    .name = "memdisk",
    .id = GRUB_DISK_DEVICE_MEMDISK_ID,
    .iterate = grub_memdisk_iterate,
    .open = grub_memdisk_open,
    .close = grub_memdisk_close,
    .read = grub_memdisk_read,
    .write = grub_memdisk_write,
    .next = 0
  };

GRUB_MOD_INIT(memdisk)
{
  if (! grub_arch_memdisk_size ())
    return;
  memdisk_addr = grub_arch_memdisk_addr ();
  grub_disk_dev_register (&grub_memdisk_dev);
}

GRUB_MOD_FINI(memdisk)
{
  if (! grub_arch_memdisk_size ())
    return;
  grub_disk_dev_unregister (&grub_memdisk_dev);
}
