#include <stdio.h>
#include <fcntl.h>

int main(void)
{		
    int fd = open("/dev/DUMMY_DEVICE", O_RDWR);

    printf("fd: %d\n", fd);
    if(fd <= 0) return -1;

    ioctl(fd, 0, NULL);

    close(fd);

    return 0;
}
