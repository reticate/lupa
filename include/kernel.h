#ifndef KERNEL_H
#define KERNEL_H

#include <linux/types.h>

uintptr_t find_kernel_symbol(const char *name);
uintptr_t scan_memory_for_pattern(uintptr_t base, size_t size, const char *pattern);

#endif
