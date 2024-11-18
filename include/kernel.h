#ifndef KERNEL_H
#define KERNEL_H

#include <linux/types.h>

uintptr_t find_kernel_symbol(const char *name);
uintptr_t disable_security_ops(void);

#endif
