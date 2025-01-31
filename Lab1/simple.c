#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gcd.h>
#include <asm/param.h>

/* Function to print the init_task PCB details *//*
void print_init_PCB(void)
{
    printk(KERN_INFO "init_task pid: %d\n", init_task.pid);
    printk(KERN_INFO "init_task state: %ld\n", init_task.state);
    printk(KERN_INFO "init_task flags: %lu\n", init_task.flags);
    printk(KERN_INFO "init_task runtime priority: %d\n", init_task.rt_priority);
    printk(KERN_INFO "init_task process policy: %d\n", init_task.policy);
    printk(KERN_INFO "init_task tgid: %d\n", init_task.tgid);
}*/

/* This function is called when the module is loaded. */
int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");

	printk(KERN_INFO "HZ = %d\n",HZ);
	printk(KERN_INFO "jiffies = %lu\n",jiffies);

       /* Print init_task details */
       //print_init_PCB();

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");

        printk(KERN_INFO "%lu\n", gcd(3300,24));
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module Printing PCB Details");
MODULE_AUTHOR("SGG");

