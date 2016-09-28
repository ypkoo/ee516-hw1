#include <stdio.h>
#include <fcntl.h>

int main(void)
{		
        char buf[20];
        int fd = open("/dev/DUMMY_DEVICE", O_RDWR);

        printf("fd: %d\n", fd);
        if(fd <= 0) return -1;

        buf[0] = 0x08;
        buf[1] = 0x09;
        buf[2] = 0x10;
        buf[3] = 0x11;

        write(fd, buf+0, 1);
	write(fd, buf+1, 1);
	write(fd, buf+2, 1);
        write(fd, buf+3, 1);
        read(fd, buf+0, 1);
        read(fd, buf+1, 1);
        read(fd, buf+2, 1);
        read(fd, buf+3, 1);
        printf("data from the device : %x\n", buf[0]);
        printf("data from the device : %x\n", buf[1]);
        printf("data from the device : %x\n", buf[2]);
        printf("data from the device : %x\n", buf[3]);

	read(fd, buf+4, 1);
        close(fd);

        return 0;
}
