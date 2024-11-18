#ifndef UTIL_H
#define UTIL_H

#include <linux/types.h>

uintptr_t find_kernel_symbol(const char *name);
uintptr_t get_kallsyms_lookup_name(void);
uintptr_t scan_memory_for_pattern(uintptr_t base, size_t size, const char *pattern);

#endif
