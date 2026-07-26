#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <errno.h>


int getSourceMac(char *ssid, unsigned char sha[6]){

  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(fd == -1){
    perror("socket");
    return -1;
  }
  struct ifreq ifr = {0};

  if(ioctl(fd, SIOCGIFHWADDR, &ifr)){
    perror("ioctl");
    close(fd);
    return -1;
  }





  return 0;
}
