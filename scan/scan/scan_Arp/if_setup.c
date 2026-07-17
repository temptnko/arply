#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include "scan_arp.h"

//#include "if_setup.h"




int maskToPrefix(uint32_t subMaskBin){
  int prefix = 0;
  for(int i = 31;i >= 0;i--){
    uint32_t arrow = 1u << i;
    
    if((subMaskBin & arrow) == 0){
      break;
    }else{
      prefix += 1;
    }
  }
  return prefix;  
}




int getWlan0Info(void){
  struct ifaddrs *ifaddr = NULL;
  struct ifaddrs *ifa = NULL;
  if(getifaddrs(&ifaddr) == -1){
    perror("getifaddrs");
    return -1;
  }

  ifa = ifaddr;
  int foundSubMask = -1;

  
  while(ifa){
    if(ifa->ifa_name && strcmp(ifa->ifa_name, "wlan0") == 0){
      struct sockaddr *takeOver = ifa->ifa_netmask;
      if(takeOver && takeOver->sa_family == AF_INET){
        struct sockaddr_in *PtakeOver = (struct sockaddr_in *)ifa->ifa_netmask;
        

        uint32_t rawSubMask = ntohl(PtakeOver->sin_addr.s_addr);
        foundSubMask = maskToPrefix(rawSubMask);
        break;
      }
    }
    ifa = ifa->ifa_next;
  }

  freeifaddrs(ifaddr);

  if(foundSubMask < 0){
    return -1;
  }else{
    return foundSubMask;
  }
}