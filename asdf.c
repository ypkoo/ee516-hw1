#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>

#include "dummy_ioctl.h"

//함수 원형 선언
static int __init dummy_init(void);
static void __exit dummy_exit(void);
ssize_t dummy_stack_read(struct file *file, char *buffer, size_t length, loff_t *offset);
ssize_t dummy_stack_write(struct file *file, const char *buffer, size_t length, loff_t *offset);
int dummy_open(struct inode *inode, struct file *file);
int dummy_release(struct inode *inode, struct file *file);
long dummy_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations dummy_fops= {
        .open=dummy_open,
        .read=dummy_stack_read,
        .write=dummy_stack_write,
        .release=dummy_release,
        .unlocked_ioctl=dummy_ioctl, //ioctl함수 사용을 위해 선언해줌
};

char devicename[20];
char value[256];
int top;
static struct cdev my_cdev;

static int __init dummy_init(void)
{
        dev_t dev=MKDEV(DUMMY_MAJOR_NUMBER,0);

	/* stack initialize */
	top=0;

        printk("Dummy Driver: init module\n");
        strcpy(devicename, "Dummy_Driver");

        register_chrdev(DUMMY_MAJOR_NUMBER, devicename, &dummy_fops);
        cdev_init(&my_cdev, &dummy_fops);
        cdev_add(&my_cdev,dev,128);
        return 0;
}


static void __exit dummy_exit(void)
{
        printk("Dummy Driver : Clean Up Module\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(MKDEV(DUMMY_MAJOR_NUMBER,0),128);
}
//user application의 요청에 따라 stack을 1byte읽은 후 전달
ssize_t dummy_stack_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{
  if(top != 0) {//stack에 element가 있을 때
    top--;
    printk("Dummy Driver : Here is Read Call[%x]\n", value[top]);
    if (copy_to_user(buffer, &value[top], sizeof(char))) //커널 공간의 Stack을 1byte 읽은 후 user space에 전달해 줌.
      return -EFAULT;
  }
  else//stack이 비었을 때
    printk("Dummy Driver : Read Call : There is nothing to read \n");  

  return 0;
}
//user application의 요청에 따라 stack에 1byte를 씀
ssize_t dummy_stack_write(struct file *file, const char *buffer, size_t length, loff_t *offset)
{
  if(top != 256){//stack이 가득 차지 않았을 때
        if( copy_from_user(&value[top], buffer, sizeof(char)))
                return -EFAULT;
	top++;
	printk("Dummy Driver: Here is Write Call [%x]\n", value[top]);
  }
  else{//stack이 가득 찼을 때
    printk("Dummy Driver: Write Call: Stack full\n");
  }
  return 0;
}

long dummy_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
  printk("Dummy Driver: IOCTL");
  switch (cmd) {
  case IOCTL_CLEAR_STACK:
    printk("IOCTL_CLEAR_STACK\n");
    top = 0;
    break;
  default:
    printk("ERROR\n");
    break;
  }
  return 0;
}

int dummy_open(struct inode *inode, struct file *file)
{
        printk("Dummy Driver : Open Call\n");
        return 0;
}


int dummy_release(struct inode *inode, struct file *file)
{
        printk("Dummy Driver : Release Call\n");
        return 0;
}

module_init(dummy_init);
module_exit(dummy_exit);

MODULE_AUTHOR("WGHAN");
MODULE_DESCRIPTION("Dummy_Driver");
MODULE_LICENSE("GPL");
