#ifndef UTIL_H
#define UTIL_H

#include <linux/types.h>

uintptr_t find_kernel_symbol(const char *name);
void clear_memory(void *addr, size_t size);
void disable_write_protection(void);
void enable_write_protection(void);

#endif
