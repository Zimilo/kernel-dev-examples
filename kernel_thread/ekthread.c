#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>

#define MYPRINT(fmt, args...)\
	printk(KERN_INFO "|%s, %i|" fmt "\n", \
	       __FUNCTION__, __LINE__, ##args);

static struct task_struct *task;

static int
ekthread_task(void *unused)
{
	printk(KERN_INFO "ekthread_task started\n");

	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ); /* Relinquish to the processor */
		MYPRINT("In the kernel");
	}

	return 0;
}

static int __init ekthread_init(void)
{
	printk(KERN_INFO "ekthread module startup\n");

	task = kthread_run(ekthread_task, NULL, "ekthreadd");
	
	if (IS_ERR(task)) {
		printk(KERN_ERR "Failed to start ekthread thread: %ld\n", 
		       PTR_ERR(task));
		return PTR_ERR(task);
	}

	return 0;
}

static void __exit ekthread_exit(void)
{
	kthread_stop(task);
	printk(KERN_INFO "ekthread module exit\n");
}

module_init(ekthread_init);
module_exit(ekthread_exit);


MODULE_AUTHOR("Rock Lee<zimilo@code-trick.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("The demo exmaple of using linux kernel threads");
