#include <grub/kernel.h>
#include <grub/memory.h>
#include <grub/machine/console.h>
#include <grub/offsets.h>
#include <grub/types.h>
#include <grub/err.h>
#include <grub/dl.h>
#include <grub/misc.h>
#include <grub/loader.h>
#include <grub/env.h>
#include <grub/cache.h>
#include <grub/time.h>
#include <grub/symbol.h>

void putchar (char c);

static const char msg[] = "Hello, world";

void
putchar (char c)
{
    asm volatile (
        "movw $0x0102, %%bx\n"
        "int $0x28\n"
    :
    :"al"(c): "bx", "memory", "cc");
}

grub_addr_t grub_modbase;

void
grub_machine_fini (int) {}

void
grub_machine_get_bootlocation (char **device __attribute__ ((unused)),
			       char **path __attribute__ ((unused)))
{
}

grub_err_t
grub_machine_mmap_iterate (grub_memory_hook_t /*hook*/, void */*hook_data*/)
{

  return 0;
}

void __attribute__ ((noreturn))
grub_exit (void)
{
  /* We can't use grub_fatal() in this function.  This would create an infinite
     loop, since grub_fatal() calls grub_abort() which in turn calls grub_exit().  */
  while (1)
    grub_cpu_idle ();
}


void
grub_machine_init (void)
{
  grub_altos_console_init ();
  const char *ptr = msg;
  while (*ptr)
    putchar (*ptr++);
  while (1);
}
