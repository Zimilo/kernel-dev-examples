#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/rbtree.h>
#include <linux/slab.h>

struct rock_node {
  struct rb_node node;
  int v;
};

struct rb_root root = RB_ROOT;
struct rock_node nodes[10];

static void __add_node_to_tree(struct rb_root *tree, struct rock_node *node)
{
  struct rb_node **new = &tree->rb_node, *parent = NULL;
  struct rock_node *rnode;
  while (*new) {
    parent = *new;
    rnode = rb_entry(*new, struct rock_node, node);
    if (rnode->v < node->v)
      new = &((*new)->rb_left);
    else if (rnode->v > node->v)
      new = &((*new)->rb_right);
    else
      return;
  }
  rb_link_node(&node->node, parent, new);
  rb_insert_color(&node->node, tree);
}

static int __init example_init(void)
{
  int i;
  struct rb_node *node;
  for (i = 0; i < 10; ++i){
    nodes[i].v = i;
    __add_node_to_tree(&root, &nodes[i]);
  }

  /* examples to iterator the tree */
  for (node = rb_first(&root); node; node = rb_next(node)) {
    printk(KERN_INFO "v = %d\n", rb_entry(node, struct rock_node, node)->v);
  }
  return 0;
}

static void __exit example_exit(void)
{
}

module_init(example_init);
module_exit(example_exit);

MODULE_AUTHOR("Rock Lee <zimilo@code-trick.com>");
MODULE_DESCRIPTION("An example of using the rbtree structure in the kernel");
MODULE_LICENSE("GPL");
