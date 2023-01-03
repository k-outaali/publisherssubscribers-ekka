#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_AUTHOR("pubsub");
MODULE_DESCRIPTION("premier driver");
MODULE_SUPPORTED_DEVICE("none");
MODULE_LICENSE("none");

static int major = 190;

module_param(major, int, 0);
MODULE_PARM_DESC(major, "major number");

static ssize_t my_read_function(struct file *file, char *buf, size_t count, loff_t *ppos)
{
    printk(KERN_DEBUG "read()\n");
    return 0;
}

static ssize_t my_write_function(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
    printk(KERN_DEBUG "write()\n");
    return 0;
}

static int my_open_function(struct inode *inode, struct file *file)
{
    printk(KERN_DEBUG "open()\n");
    return 0;
}

static int my_release_function(struct inode *inode, struct file *file)
{
    printk(KERN_DEBUG "close()\n");
    return 0;
}

struct file_operations fops =
{
    .read = my_read_function,
    .write = my_write_function,
    .open = my_open_function,
    .release = my_release_function  /* correspond a close */
};

static int __init mon_module_init(void)
{
    int ret;

    ret = register_chrdev(major, "pubsub", &fops);
    
    if(ret < 0)
    {
        printk(KERN_WARNING "Probleme sur le major\n");
        return ret;
    }
    
    printk(KERN_DEBUG "mydriver chargé avec succès\n");
    return 0;
}

static void __exit mon_module_cleanup(void)
{
    unregister_chrdev(major, "pubsub");
    
    printk(KERN_DEBUG "pubsub déchargé avec succès\n");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);