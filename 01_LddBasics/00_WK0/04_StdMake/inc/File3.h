#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>


extern int my_proc_open2(struct inode *inode, struct file *file);
extern int my_proc_release2(struct inode *inode, struct file *file);
extern ssize_t	MyReadFunc2(struct file *fp, char __user *UserData, size_t Size, loff_t * Offset);
