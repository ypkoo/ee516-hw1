# TARGET = dummy
# dummy-objs := stack.o dummy_driver.o

# stack.o : stack.c stack.h
# 	gcc -c -o stack.o stack.c

# dummy_driver.o : dummy_driver.c stack.c stack.h
# 	gcc -c -o dummy_driver.o dummy_driver.c

# SRCS   = m1.c m2.c
# OBJS   = $(SRCS:.c=.o)

# obj-m += $(OBJS)

SRCS = stack.c dummy_driver.c
OBJS = $(SRCS:.c=.o)

obj-m += $(OBJS)

KERNEL_DIR :=/lib/modules/$(shell uname -r)/build
PWD :=$(shell pwd)

default:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) clean