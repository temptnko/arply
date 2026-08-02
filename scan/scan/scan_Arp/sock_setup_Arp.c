
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

struct EthernetFrame buildFrame(const uint8_t source[6], const uint32_t spa, const uint32_t tpa){ 
  struct EthHeader eth = {0};
  
  eth.type = htons(ETH_P_ARP);
  memcpy(eth.source, source, 6);
  memcpy(eth.destination, "\xff\xff\xff\xff\xff\xff", 6);

  struct ArpHeader arp = {0};

  arp.htype = htons(1);
  arp.ptype = htons(ETH_P_IP);
  arp.hlen = 6;
  arp.plen = 4;
  arp.oper = htons(1);
  arp.tha = {0, 0, 0, 0, 0, 0};
  memcpy(arp.sha, source, 6);
  arp.tpa = htonl(tpa);
  arp.spa = htonl(spa);

  struct EthernetFrame frame = {0};
    frame.eth = eth;
    frame.arp = arp;

  return frame; 
}
