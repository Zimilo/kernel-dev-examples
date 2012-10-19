#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init helloworld_init(void)
{
	printk(KERN_INFO "helloworld module said hello!\n");
	return 0;
}

static void __exit helloworld_exit(void)
{
	printk(KERN_INFO "helloworld module said goodbye!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_AUTHOR("Rock Lee<zimilo@code-trick.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("The helloworld module");
