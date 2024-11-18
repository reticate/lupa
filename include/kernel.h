#ifndef KERNEL_H
#define KERNEL_H

#include <linux/types.h>

uintptr_t find_kernel_symbol(const char *name);
uintptr_t get_kallsyms_lookup_name(void);
uintptr_t get_selinux_ops(void);
uintptr_t get_apparmor_ops(void);

#endif
