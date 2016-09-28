#include<stdio.h>
#include<fcntl.h>

int main(void)
{
  char buf[1];
  int fd = open("/dev/DUMMY_DEVICE",O_RDWR);
  
  if(fd<=0)
    return -1;
  buf[0] = 'a';
  write(fd,buf,sizeof(buf));
  close(fd);

  return 0;
}
