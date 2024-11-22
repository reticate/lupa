#include "util.h"
#include "logging.h"
#include <linux/workqueue.h>
#include <linux/smp.h>
#include <linux/version.h>

struct scan_task {
    uintptr_t base;
    size_t size;
    const char *pattern;
    uintptr_t *result;
    struct completion *done;
};

static void scan_memory_worker(struct work_struct *work) {
    struct scan_task *task = container_of(work, struct scan_task, work);
    size_t i;

    for (i = 0; i < task->size; i++) {
        uintptr_t addr = task->base + i;
        if (memcmp((void *)addr, task->pattern, strlen(task->pattern)) == 0) {
            *task->result = addr;
            break;
        }
    }

    complete(task->done);
}

uintptr_t scan_memory_for_pattern(uintptr_t base, size_t size, const char *pattern) {
    struct scan_task tasks[NR_CPUS];
    struct work_struct work_items[NR_CPUS];
    uintptr_t chunk_size = size / NR_CPUS;
    struct completion completions[NR_CPUS];
    uintptr_t result = 0;
    int cpu;

    for_each_online_cpu(cpu) {
        init_completion(&completions[cpu]);
        tasks[cpu].base = base + cpu * chunk_size;
        tasks[cpu].size = (cpu == NR_CPUS - 1) ? size - (cpu * chunk_size) : chunk_size;
        tasks[cpu].pattern = pattern;
        tasks[cpu].result = &result;
        tasks[cpu].done = &completions[cpu];

        INIT_WORK(&work_items[cpu], scan_memory_worker);
        schedule_work_on(cpu, &work_items[cpu]);
    }

    for_each_online_cpu(cpu) {
        wait_for_completion(&completions[cpu]);
        if (result) break;
    }

    for_each_online_cpu(cpu) {
        cancel_work_sync(&work_items[cpu]);
    }

    return result;
}

uintptr_t get_kallsyms_lookup_name(void) {
    uintptr_t base = 0xffffffff81000000;
    size_t scan_size = 0x200000;
    const char *pattern = "kallsyms_lookup_name";
    uintptr_t result = scan_memory_for_pattern(base, scan_size, pattern);
    if (!result) return 0;
    return result;
}

uintptr_t get_selinux_ops(void) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 0, 0)
    return find_kernel_symbol("security_hook_heads");
#else
    return find_kernel_symbol("selinux_ops");
#endif
}

uintptr_t get_apparmor_ops(void) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 0, 0)
    return find_kernel_symbol("lsm_hook_heads");
#else
    return find_kernel_symbol("apparmor_ops");
#endif
}

uintptr_t find_kernel_symbol(const char *name) {
    uintptr_t kallsyms_lookup = get_kallsyms_lookup_name();
    if (!kallsyms_lookup) return 0;
    return ((uintptr_t (*)(const char *))kallsyms_lookup)(name);
}
