#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int getSourceMac(char *ssid, uint8_t sha[6]){

  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(fd == -1){
    perror("socket");
    return -1;
  }
  struct ifreq ifr = {0};

  strcpy(ifr.ifr_name, ssid);
  ifr.ifr_name[IFNAMSIZ - 1] = '\0';

  if(ioctl(fd, SIOCGIFHWADDR, &ifr)){
    perror("ioctl");
    close(fd);
    return -1;
  }

  memcpy(sha, ifr.ifr_hwaddr.sa_data, 6);

  close(fd);
  return 0;
}
