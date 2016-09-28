#include <stdio.h>
#include <fcntl.h>

int main(void)
{		
    char buf[1];
    int fd = open("/dev/DUMMY_DEVICE", O_RDWR);

    printf("fd: %d\n", fd);
    if(fd <= 0) return -1;

    read(fd, buf, 1);

    close(fd);

    return 0;
}

