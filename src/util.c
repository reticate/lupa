#include "util.h"
#include "logging.h"

uintptr_t get_kallsyms_lookup_name(void) {
    LOG_INFO("Locating kallsyms_lookup_name symbol.\n");
    uintptr_t base = 0xffffffff81000000;
    size_t scan_size = 0x200000;
    const char *pattern = "kallsyms_lookup_name";
    uintptr_t offset = scan_memory_for_pattern(base, scan_size, pattern);
    if (!offset) {
        LOG_WARN("kallsyms_lookup_name not found.\n");
        return 0;
    }
    LOG_DEBUG("kallsyms_lookup_name found at offset: %lx\n", offset);
    return base + offset;
}

uintptr_t find_kernel_symbol(const char *name) {
    LOG_INFO("Resolving kernel symbol: %s\n", name);
    uintptr_t kallsyms_lookup = get_kallsyms_lookup_name();
    if (!kallsyms_lookup) {
        LOG_WARN("Failed to resolve kallsyms_lookup_name.\n");
        return 0;
    }
    uintptr_t symbol_addr = ((uintptr_t (*)(const char *))kallsyms_lookup)(name);
    if (!symbol_addr) {
        LOG_WARN("Symbol not found: %s\n", name);
    } else {
        LOG_DEBUG("Symbol resolved: %s -> %p\n", name, (void *)symbol_addr);
    }
    return symbol_addr;
}
