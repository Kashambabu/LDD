#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include "File2.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BABU_K - LDD");
MODULE_DESCRIPTION("Our first dynamically loadable kernel module");


#define PROC_NAME "FirstDriverFile"
#define BUFFER_SIZE 128


static struct proc_dir_entry *UserProcDirEntry;

extern int my_proc_open(struct inode *inode, struct file *file);
extern int my_proc_release(struct inode *inode, struct file *file);
extern ssize_t	MyReadFunc(struct file *fp, char __user *UserData, size_t Size, loff_t * Offset);


static struct proc_ops UserProcOps = 
{
    .proc_read = MyReadFunc,
    .proc_open = my_proc_open,
    .proc_release = my_proc_release,
};


static int __init my_init(void) 
{
    printk("Hello: Init Kernel Start\n");

    UserProcDirEntry = proc_create(PROC_NAME, 0U, NULL, &UserProcOps);

    printk("Hello: Init Kernel Stop\n");

    return 0;
}

static void __exit my_exit(void)
{
    printk("Hello: Exit Kernel Start\n");
    proc_remove(UserProcDirEntry);
    printk("Hello: Exit Kernel Stop\n");

}

module_init(my_init);
module_exit(my_exit);