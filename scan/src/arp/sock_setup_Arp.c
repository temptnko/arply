/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <net/if_arp.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h> 
*/

#include "sock_setup_Arp.h"

int settupSocket(){
    int packetSock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if(packetSock < 0){
        perror("socket");
        return -1;
    }
    return packetSock;
}

struct arp_packet buildFrame(const uint8_t source[6], const uint32_t spa, const uint32_t tpa){ 
  struct ethhdr eth = {0};
  
  eth.type = htons(ETH_P_ARP);
  memcpy(eth.source, source, 6);
  memcpy(eth.destination, "\xff\xff\xff\xff\xff\xff", 6);

  struct arp_hdr arp = {0};

  arp.htype = htons(1);
  arp.ptype = htons(ETH_P_IP);
  arp.hlen = 6;
  arp.plen = 4;
  arp.oper = htons(1);
  memcpy(arp.sha, source, 6);
  arp.tpa = htonl(tpa);
  arp.spa = htonl(spa);

  struct arp_packet frame = {0};
    frame.eth = eth;
    frame.arp = arp;

  return frame; 
}
