#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("BABU_K - LDD");
MODULE_DESCRIPTION("Kernel Logging Methods");

static char ModuleName[] = "KernelLog";

static int __init KernelLog_init(void) 
{
    printk(KERN_DEBUG "%s: Debug: Init Kernel Stop\n",ModuleName);
    printk(KERN_INFO "%s: Info: Init Kernel Start\n",ModuleName);
    printk(KERN_NOTICE "%s: Notice: Init Kernel Stop\n",ModuleName);
    printk(KERN_WARNING "%s: Warning: Init Kernel Stop\n",ModuleName);
    printk(KERN_ERR "%s: Error: Init Kernel Stop\n",ModuleName);
    printk(KERN_CRIT "%s: Critical: Init Kernel Stop\n",ModuleName);
    printk(KERN_ALERT "%s: Alert: Init Kernel Stop\n",ModuleName);
    printk(KERN_EMERG "%s: Emerg: Init Kernel Stop\n",ModuleName);
    printk(KERN_INFO "The process is \"%s\" (pid %i)\n",current->comm, current->pid);

    return 0;
}

static void __exit KernelLog_exit(void)
{
    pr_emerg("%s: Emerg: Exit Kernel Start\n",ModuleName);
    pr_alert("%s: Alert: Exit Kernel Stop\n",ModuleName);
    pr_crit("%s: Critical: Exit Kernel Stop\n",ModuleName);
    pr_err("%s: Error: Exit Kernel Stop\n",ModuleName);
    pr_warn("%s: Warning: Exit Kernel Stop\n",ModuleName);
    pr_notice("%s: Notice: Exit Kernel Stop\n",ModuleName);
    pr_info("%s: Info: Exit Kernel Stop\n",ModuleName);
    pr_debug("%s: Debug: Exit Kernel Stop\n",ModuleName);
}

module_init(KernelLog_init);
module_exit(KernelLog_exit);