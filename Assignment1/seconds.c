/*
SFWRENG 3SH3 Assignment 1

Group 24:
Mehak Shah, shahm77, 400478491
Samridhi Anand, anands29, 400478945

Assignment 1 Goal: 
Design a kernel module that creates a /proc file named /proc/seconds that 
reports the number of elapsed seconds since the kernel module was loaded.

Linux Distribution Information:

Distributor ID: Ubuntu
Description:    Ubuntu 16.04.7 LTS
Release:        16.04
Codename:       xenial
Kernel version: 4.4.0-210-generic
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

// Store jiffies
static unsigned long start_jiffies;

// Function prototype
ssize_t proc_seconds_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

// File operations for proc entry (in older kernels)
static struct file_operations proc_seconds_fops = {
    .owner = THIS_MODULE,
    .read = proc_seconds_read,
};

// Function called when module is loaded
int proc_init(void)
{
    start_jiffies = jiffies; // Store initial jiffies

    // Create /proc/seconds entry
    proc_create(PROC_NAME, 0, NULL, &proc_seconds_fops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

// Function called when the module is removed
void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

// Function called each time /proc/seconds is read
ssize_t proc_seconds_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    unsigned long elapsed_seconds;

    // Prevent multiple reads in a single call
    if (*pos > 0) {
        return 0;  // End of file
    }

    // Calculate elapsed time
    elapsed_seconds = (jiffies - start_jiffies) / HZ;

    // Output time to buffer
    rv = sprintf(buffer, "Elapsed seconds: %lu\n", elapsed_seconds);

    // Copy data to user space
    copy_to_user(usr_buf, buffer, rv);

    // Update file position
    *pos = rv;

    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel module to report elapsed seconds since module load");
MODULE_AUTHOR("Group 24");