#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
//#include "scan_arp.h"
#include "if_setup.h"


//uint8_t  sha[6];
//uint8_t  spa[4];



//const char *command ="nmcli -t -f DEVICE,TYPE,STATE dev status | awk -F: '$2==\"wifi\" && $3==\"connected\"{print $1; exit}'";
int getCommandOutput(const char *command, char *buf, size_t bufSize){
  if(!command || !buf || bufSize == 0){
    return -1;
  }
  buf[0] = '\0'; 
  FILE *cmd = popen(command, "r");
  if(!cmd)return -1;

  if(!fgets(buf, bufSize, cmd)){
    pclose(cmd);
    buf[0] = '\0';
    return -1;
  }
  
  buf[strcspn(buf, "\r\n")] = '\0';

  pclose(cmd);  
  return 0;
}

uint8_t maskToPrefix(uint32_t subMaskBin){
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

int getIfInfo(uint8_t *subMask, uint32_t *sourceIp, const char *ssid){
  if(!subMask || !sourceIp)return -1;
  *subMask = 0;
  *sourceIp = 0;
  struct ifaddrs *ifaddr = NULL;
  struct ifaddrs *ifa = NULL;
  if(getifaddrs(&ifaddr) == -1){
    perror("getifaddrs");
    return -1;
  }

  ifa = ifaddr;
  int status = 0;
  
  while(ifa){
    if(ifa->ifa_name && strcmp(ifa->ifa_name, ssid) == 0){
      if(ifa->ifa_netmask && ifa->ifa_netmask->sa_family == AF_INET){
        struct sockaddr_in *Psub = (struct sockaddr_in *)ifa->ifa_netmask;
        

        uint32_t rawSubMask = ntohl(Psub->sin_addr.s_addr);
        *subMask = maskToPrefix(rawSubMask);
      }
      if(ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET){
        struct sockaddr_in *Pip = (struct sockaddr_in *)ifa->ifa_addr;
        *sourceIp = ntohl(Pip->sin_addr.s_addr);
      }
      if(*sourceIp != 0 && *subMask != 0)break;
    }
    ifa = ifa->ifa_next;
  }

  if(*subMask == 0)status -= 1;
  if(*sourceIp == 0)status -= 2;
  freeifaddrs(ifaddr);
  return status;
}
