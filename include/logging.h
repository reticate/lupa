#ifndef LOGGING_H
#define LOGGING_H

#include <linux/printk.h>

#define LOG_INFO(fmt, ...) printk(KERN_INFO "shim: " fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printk(KERN_ERR "shim: " fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) printk(KERN_DEBUG "shim: " fmt, ##__VA_ARGS__)

#endif
