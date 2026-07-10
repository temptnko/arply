
#include <stdio.h>          // standard i/o: perror, printf, file, etc.
#include <stdlib.h>         // general utilities: exit, malloc, free, atoi, strtol
#include <string.h>         // string/memory helpers: memcpy, memset, strlen, strcmp
#include <errno.h>          // handles errors
#include <sys/types.h>      // system typedefs: ssize_t, pid_t, etc.
#include <sys/socket.h>     // core socket apis/types: socket(), bind(), struct sockaddr
#include <netinet/in.h>     // internet utilities: htons(), ntohs(), sockaddr_in (ip sockets)
#include <linux/if_packet.h> // packet socket layer: af_packet, struct sockaddr_ll (raw link-layer sockets)
#include <linux/if_ether.h>  // ethernet constants: eth_p_arp, eth_alen, eth_hlen
#include <linux/if_arp.h>   // arp/hardware address types: arphrd_ether, arphrd_* constants
#include <unistd.h> //close
#include <net/if.h> // if_nametoindex
#include <stdint.h> //uintx_t
#include <arpa/inet.h> //htons
//#include "scanlib.h"
#include <ifaddrs.h> //getifaddress




uint32_t maskToPrefix(uint32_t subMaskBin){
  int prefix = 0;
  for(int i = 31;i != 0;i--){
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

  int found = 0;
  while(ifa){
    if(ifa->ifa_name && strcmp(ifa->ifa_name, "wlan0") == 0){
      struct sockaddr *takeOver = ifa->ifa_netmask;
      if(takeOver && takeOver->sa_family == AF_INET){
        struct sockaddr_in *PtakeOver = (struct sockaddr *)takeOver;
        struct in_addr freshSubMask = PtakeOver->sin_addr;
        

        uint32_t rawSubMask = ntohl(freshSubMask.s_addr);
        int subMask = maskToPrefix(rawSubMask);
        found = 1;
        break;
      }


    }
    ifa = ifa->ifa_next;
  }


  if(!found){
    perror("wlan0 not found");
  }

  freeifaddrs(ifaddr);
}




int settupSocket(void){
    int packetSock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if(packetSock < 0){
        perror("socket");
        return -1;
    }

    struct sockaddr_ll sll = {0};
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ARP);
    sll.sll_ifindex = if_nametoindex("wlan0");
    if(sll.sll_ifindex == 0){
        perror("if_nametoindex");
        close(packetSock);
        return -1;
    }
    sll.sll_hatype = ARPHRD_ETHER;
    sll.sll_pkttype = 0;
    sll.sll_halen = ETH_ALEN;
    memcpy(sll.sll_addr, "\xff\xff\xff\xff\xff\xff", 6);
    socklen_t addrlen = sizeof(struct sockaddr_ll);

    if(bind(packetSock, &sll, addrlen) == -1){
        perror("bind");
        close(packetSock);
        return -1;
    }
    return packetSock;
}

struct EthernetFrame buildFrame(const uint8_t destination[6], const uint8_t source[6], const uint8_t sha[6], const uint8_t spa[4], const uint8_t tpa[4]){ 
  struct EthernetFrame frame = {0};
  struct EthernetFrame frame = {
    .type        = htons(ETH_P_ARP),

    .htype       = htons(1),
    .ptype       = htons(ETH_P_IP),
    .hlen        = 6,
    .plen        = 4,
    .oper        = htons(1),
    .tha         = {0, 0, 0, 0, 0, 0}
  };

  memcpy(frame.destination, destination, 6);
  memcpy(frame.source, source, 6);
  memcpy(frame.sha, sha, 6);
  memcpy(frame.spa, spa, 4);//memcpy(struct of &x, struct or &y, sizeof(copied data)); copies data from y to x 
  memcpy(frame.tpa, tpa, 4); 

  return frame; 
}
