#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#define DEVICE_NAME "pubsub"
#define MINOR_COUNT 4

struct message {
    char *data;
    size_t length;
};

struct pubsub_dev {
    struct message *msg;
    struct cdev cdev;
};

static dev_t dev;
static struct pubsub_dev *pubsub_devices;

/* file operations */

static int device_open(struct inode *inode, struct file *file)
{
    struct pubsub_dev *dev;

    /* get a  pointer to the device data */
    dev = container_of(inode->i_cdev, struct pubsub_dev, cdev);
    file->private_data = dev;
        printk(KERN_INFO "pubsub: open ()\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{           printk(KERN_INFO "pubsub: close ()\n");
    return 0;
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
    struct pubsub_dev *dev = filp->private_data;
    size_t ret = 0;

/* return the message to the user */
    if (length < dev->msg->length)
        return -EINVAL;
    if(raw_copy_to_user(buffer, dev->msg->data, dev->msg->length))
        return -EFAULT;
    ret = dev->msg->length;

    return ret;
}

static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset)
{
    struct pubsub_dev *dev = filp->private_data;

    /* copy the data from the user buffer */
    if(length > PAGE_SIZE)
        length = PAGE_SIZE;
    if (raw_copy_from_user(dev->msg->data, buffer, length))
        return -EFAULT;
    dev->msg->length = length;

    /* send the message to the subscribers */
    /* ... */

    return length;
}

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

/* module initialization and cleanup */

static int __init pubsub_init(void)
{
    int ret,i;

/* allocate memory for the devices */
    pubsub_devices = kmalloc(MINOR_COUNT * sizeof(*pubsub_devices), GFP_KERNEL);
    if (!pubsub_devices) {
    printk(KERN_ERR "pubsub: unable to allocate memory\n");
        return -ENOMEM;
    }

    /* allocate memory for the messages */
    for(i = 0; i < MINOR_COUNT; i++) {
        pubsub_devices[i].msg = kmalloc(sizeof(*pubsub_devices[i].msg), GFP_KERNEL);
        if (!pubsub_devices[i].msg) {
            printk(KERN_ERR "pubsub: unable to allocate memory\n");
            goto error;
        }
        pubsub_devices[i].msg->data = kmalloc(PAGE_SIZE, GFP_KERNEL);
        if (!pubsub_devices[i].msg->data) {
            printk(KERN_ERR "pubsub: unable to allocate memory\n");
            kfree(pubsub_devices[i].msg);
            goto error;
        }
    }

    /* allocate a major and minor number for the devices */
    ret = alloc_chrdev_region(&dev, 0, MINOR_COUNT, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "pubsub: unable to allocate device numbers\n");
        goto error;
    }

    /* initialize the cdev structures and add them to the kernel */
    for(i = 0; i < MINOR_COUNT; i++) {
        cdev_init(&pubsub_devices[i].cdev, &fops);
        pubsub_devices[i].cdev.owner = THIS_MODULE;
        ret = cdev_add(&pubsub_devices[i].cdev, dev + i, 1);
        if (ret < 0) {
            printk(KERN_ERR "pubsub: unable to add cdev\n");
            goto error;
        }
    }

    printk(KERN_INFO "pubsub: devices registered with major number %d and  minor number%d \n", MAJOR(dev),MINOR(dev));
    return 0;

error:
    /* cleanup on error */
    for (i = 0; i < MINOR_COUNT; i++) {
        if (pubsub_devices[i].msg) {
            if (pubsub_devices[i].msg->data)
                vfree(pubsub_devices[i].msg->data);
            vfree(pubsub_devices[i].msg);
        }
    }
    vfree(pubsub_devices);
    return -ENOMEM;
}

static void __exit pubsub_exit(void)
{
int i;
    /* remove the cdev structures and release the device numbers */
    for (i = 0; i < MINOR_COUNT; i++)
        cdev_del(&pubsub_devices[i].cdev);
   
unregister_chrdev_region(dev, MINOR_COUNT);

    /* free the memory */
    for (i = 0; i < MINOR_COUNT; i++) {
        kfree(pubsub_devices[i].msg->data);

        kfree(pubsub_devices[i].msg);
    }
    kfree(pubsub_devices);
    printk(KERN_INFO "pubsub: devices unregistered avec sucess\n");
}

module_init(pubsub_init);
module_exit(pubsub_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EKKER");
MODULE_DESCRIPTION("Publisher-subscriber device driver");
