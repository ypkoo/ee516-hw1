#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>

#define DUMMY_MAJOR_NUMBER 250

#define MAX_SIZE 256

typedef struct stack {
                int top;
                char item[MAX_SIZE];
} stack_t;

int stack_init(stack_t *s) {
        if (s == NULL) return -1;

        s->top = 0;

        return 0;
}

int is_full(stack_t *s) {
        return (s->top == MAX_SIZE - 1);
}

int is_empty(stack_t *s) {
        return (s->top == 0);
}

int push(stack_t *s, const char *value) {
        if (is_full(s) || s == NULL || value == NULL) return -1;

        s->item[s->top] = *value;
        s->top++;

        return 0;
}

int pop(stack_t *s, char *value) {

        if (is_empty(s) || s == NULL || value == NULL) return -1;

        *value = s->item[s->top];
        s->top--;

        return 0;
}

char top(stack_t *s) {
        if (is_empty(s) || s == NULL) return -1;

        // *value = s->item[s->top];

        return s->item[s->top];
}



static int __init dummy_init(void);
static void __exit dummy_exit(void);
ssize_t dummy_read(struct file *file, char *buffer, size_t length, loff_t *offset);
ssize_t dummy_write(struct file *file, const char *buffer, size_t length, loff_t *offset);
int dummy_open(struct inode *inode, struct file *file);
int dummy_release(struct inode *inode, struct file *file);

static struct file_operations dummy_fops= {
        .open=dummy_open,
        .read=dummy_read,
        .write=dummy_write,
        .release=dummy_release,
};

char devicename[20];
stack_t stack; // a stack to use.

// char value=' ';
static struct cdev my_cdev;

static int __init dummy_init(void)
{
        dev_t dev=MKDEV(DUMMY_MAJOR_NUMBER,0);

        printk("Dummy Driver: init module\n");
        strcpy(devicename, "Dummy_Driver");

        register_chrdev(DUMMY_MAJOR_NUMBER, devicename, &dummy_fops);
        cdev_init(&my_cdev, &dummy_fops);
        cdev_add(&my_cdev,dev,128);

        /* stack initialization */
        if (stack_init(&stack) != 0) {
                printk("Stack initialization failed.\n");
        };

        return 0;
}


static void __exit dummy_exit(void)
{
        printk("Dummy Driver : Clean Up Module\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(MKDEV(DUMMY_MAJOR_NUMBER,0),128);
}


ssize_t dummy_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{
        if (!is_empty(&stack)) {
                if (pop(&stack, buffer) != 0)
                        return -EFAULT;
                else
                        printk("Dummy Driver : Here is Read Call[%x]\n", top(&stack));
                // if (copy_to_user(buffer, &(top(stack)), sizeof(char)))
                //         return -EFAULT;
                
        }
        else {
                printk("Dummy Driver: Stack is empty.\n");
        }
        
        return 0;
}

ssize_t dummy_write(struct file *file, const char *buffer, size_t length, loff_t *offset)
{
        // char vaule;

        if (!is_full(&stack)) {
                // if( copy_from_user(&value, buffer, sizeof(char)))
                //         return -EFAULT;
                if (push(&stack, buffer) != 0) 
                        return -EFAULT;
                else
                        printk("Dummy Driver: Here is Write Call [%x]\n", top(&stack));
        }
        else {
                printk("Dummy Driver: Stack is full.\n");
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

MODULE_AUTHOR("YPKOO");
MODULE_DESCRIPTION("Dummy_Driver");
MODULE_LICENSE("GPL");
