#include <stdio.h>
#include <fcntl.h>

int main(void)
{
        char buf[20];
        int fd = open("/dev/DUMMY_DEVICE", O_RDWR);

        if(fd <= 0) return -1;

        buf[0] = 0x07;
        write(fd, buf, 1);
        read(fd, buf, 1);
        printf("data from the device : %x\n", buf[0]);
        close(fd);

        return 0;
}