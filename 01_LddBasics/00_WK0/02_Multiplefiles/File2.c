#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>


#define PROC_NAME "FirstDriverFile"
#define BUFFER_SIZE 128


/* Custom structure to hold context for each individual file open instance */
struct my_file_context {
    char data[BUFFER_SIZE];
    size_t data_len;
};

unsigned int MyReadCounter =0u;

int my_proc_open(struct inode *inode, struct file *file);
int my_proc_release(struct inode *inode, struct file *file);
ssize_t	MyReadFunc(struct file *fp, char __user *UserData, size_t Size, loff_t * Offset);

/* Executed when a user opens /proc/file_context_demo */
int my_proc_open(struct inode *inode, struct file *file)
{
    struct my_file_context *ctx;

    // Allocate memory unique to this specific file descriptor session
    ctx = kmalloc(sizeof(struct my_file_context), GFP_KERNEL);
    if (!ctx)
        return -ENOMEM;

    // Generate a unique dynamic string containing the current process ID (PID)
    ctx->data_len = snprintf(ctx->data, BUFFER_SIZE, 
                             "Hello! You are reading via PID: %d\n", current->pid);

    // Save our custom context directly inside the struct file pointer
    file->private_data = ctx;

    printk("Hello: Opened by PID %d, context allocated\n", current->pid);
    return 0;
}

/* Executed when the user closes the file descriptor (e.g., cat finishes) */
int my_proc_release(struct inode *inode, struct file *file)
{
    // Clean up the memory allocated for this specific open instance
    struct my_file_context *ctx = file->private_data;
    
    if (ctx) {
        kfree(ctx);
        file->private_data = NULL; // Prevent dangling pointers
    }

    printk("proc_example: File closed, context freed\n");
    return 0;
}

ssize_t	MyReadFunc(struct file *fp, char __user *UserData, size_t Size, loff_t * Offset)
{
    char msg[] = "DummyData\n";
    ssize_t len = strlen(msg);
    unsigned long retval;
    ssize_t retLen;

    MyReadCounter++;
    printk("Hello: My Read Function Calling Counts:%d \n",MyReadCounter);
    printk("Hello: My Read Function Calling Offset:%lld \n",*Offset);

    if(*Offset<len)
    {
        retval = copy_to_user(UserData, msg, len);
        retLen = len;
    }
    else
    {
        retLen = 0U;   
    }
    
    *Offset += len;

   return retLen; 
}
