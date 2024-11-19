#ifndef SHIM_H
#define SHIM_H

#include <linux/types.h>

void hook_kernel(void);
void disable_lsm_hooks(void);
void restore_lsm_hooks(void);

#endif
