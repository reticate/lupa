#ifndef SHIM_H
#define SHIM_H

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>

void hook_kernel(void);
void disable_lsm_hooks(void);

#endif
