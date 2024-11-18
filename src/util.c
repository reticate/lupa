#include "util.h"
#include "mem_utils.h"
#include <linux/string.h>

uintptr_t find_kernel_symbol(const char *name) {
    uintptr_t kallsyms_lookup_name = get_kallsyms_lookup_name();
    if (!kallsyms_lookup_name) return 0;
    return ((uintptr_t (*)(const char *))kallsyms_lookup_name)(name);
}

void clear_memory(void *addr, size_t size) {
    memset(addr, 0, size);
}

void disable_write_protection(void) {
    uintptr_t cr0 = read_cr0();
    write_cr0(cr0 & ~0x00010000);
}

void enable_write_protection(void) {
    uintptr_t cr0 = read_cr0();
    write_cr0(cr0 | 0x00010000);
}
