
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <unistd.h>
#include <net/if.h>
#include <stdint.h>
#include <arpa/inet.h> 
#include "sock_setup_Arp.h"




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
