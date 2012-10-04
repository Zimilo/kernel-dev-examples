#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define EXAMPLE_DIR "example"
#define TUNNING_ENTRY "tunning"

struct tunning_data {
  int v;
};

static struct proc_dir_entry *example_dir = NULL;
static struct tunning_data t = {10};

int read_tunning(char *page, char **start, off_t off,
		 int count, int *eof, void *data)
{
  int len;
  struct tunning_data *td = (struct tunning_data *)data;
  try_module_get(THIS_MODULE);
  len = sprintf(page, "tunning value = %d\n", td->v);
  module_put(THIS_MODULE);
  return len;
}

int write_tunning(struct file *file, const char __user *buffer,
		  unsigned long count, void *data)
{
  struct tunning_data *td = (struct tunning_data *)data;
  int len;
  char buf[8];
  try_module_get(THIS_MODULE);

  if (count > 6) 
    len = 6;
  else
    len = count;

  if (copy_from_user(buf, buffer, len)) {
    module_put(THIS_MODULE);
    return -EFAULT;
  }
  
  buf[len] = '\0';
  
  if (kstrtoint(buf, 10, &td->v) < 0) {
    module_put(THIS_MODULE);
    return -EFAULT;
  }

  module_put(THIS_MODULE);
  return len;
}

static int __init example_init(void)
{

  struct proc_dir_entry *entry;
  int rv = 0;

  printk(KERN_INFO "proc fs example init\n");

  example_dir = proc_mkdir(EXAMPLE_DIR, NULL);
  if (!example_dir) {
    rv = -ENOMEM;
    goto out;
  }

  entry = create_proc_entry(TUNNING_ENTRY, 0666, example_dir);
  if (!entry) {
    rv = -ENOMEM;
    goto no_entry;
  }

  entry->read_proc = read_tunning;
  entry->write_proc = write_tunning;
  entry->data = &t;

  goto out;
 no_entry:
  remove_proc_entry(EXAMPLE_DIR, NULL);
 out:
  return rv;
}

static void __exit example_exit(void)
{
  printk(KERN_INFO "proc fs example exit\n");
  remove_proc_entry(TUNNING_ENTRY, example_dir);
  remove_proc_entry(EXAMPLE_DIR, NULL);
}

module_init(example_init);
module_exit(example_exit);

MODULE_AUTHOR("Rock Lee<Zimilo@code-trick.com>");
MODULE_DESCRIPTION("An example interactived with procfs");
MODULE_LICENSE("GPL");
