#include "util.h"
#include <linux/string.h>

uintptr_t get_kallsyms_lookup_name(void) {
    uintptr_t base = 0xffffffff81000000;
    size_t scan_size = 0x200000;
    const char *pattern = "kallsyms_lookup_name";
    uintptr_t offset = scan_memory_for_pattern(base, scan_size, pattern);
    if (!offset) return 0;
    return base + offset;
}

uintptr_t find_kernel_symbol(const char *name) {
    uintptr_t kallsyms_lookup = get_kallsyms_lookup_name();
    if (!kallsyms_lookup) return 0;
    return ((uintptr_t (*)(const char *))kallsyms_lookup)(name);
}

uintptr_t scan_memory_for_pattern(uintptr_t base, size_t size, const char *pattern) {
    for (size_t i = 0; i < size; i++) {
        uintptr_t addr = base + i;
        if (memcmp((void *)addr, pattern, strlen(pattern)) == 0) return i;
    }
    return 0;
}
