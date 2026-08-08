#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

int getSourceInfo(char *ssid, uint8_t sha[6], uint32_t *sourceIp, uint32_t *subMask){
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(fd == -1){
    perror("socket");
    return -1;
  }
  struct ifreq ifr = {0};

  strncpy(ifr.ifr_name, ssid, IFNAMSIZ - 1);
  ifr.ifr_name[IFNAMSIZ - 1] = '\0';

  if(ioctl(fd, SIOCGIFHWADDR, &ifr) < 0){
    perror("ioctl (SIOCGIFHWADDR) failed");
    close(fd);
    return -1;
  }

  memcpy(sha, ifr.ifr_hwaddr.sa_data, 6);


  if(ioctl(fd, SIOCGIFNETMASK, &ifr) < 0){
    perror("ioctl (SIOCGIFNETMASK) failed");
    close(fd);
    return -1;
  }

  struct sockaddr_in *sub = (struct sockaddr_in*)&ifr.ifr_netmask;
  *subMask = ntohl(sub->sin_addr.s_addr);


  if(ioctl(fd, SIOCGIFADDR, &ifr) < 0){
    perror("ioctl (SIOCGIFADDR) failed");
    close(fd);
    return -1;
  }
  
  struct sockaddr_in *ip = (struct sockaddr_in*)&ifr.ifr_addr;
  *sourceIp = ntohl(ip->sin_addr.s_addr);

  close(fd);
  return 0;
}
