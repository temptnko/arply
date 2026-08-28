#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "arp/sock_setup_arp.h"

int settupSocket(){
    int packetSock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if(packetSock < 0){
        perror("socket");
        return -1;
    }
    return packetSock;
}

struct arp_packet buildFrame(uint8_t source[6], uint32_t spa, uint32_t tpa){ 
  struct ethhdr eth = {0};
  
  eth.h_proto = htons(ETH_P_ARP);
  memcpy(eth.h_source, source, 6);
  memcpy(eth.h_dest, "\xff\xff\xff\xff\xff\xff", 6);

  struct arphdr arp = {0};

  arp.htype = htons(1);
  arp.ptype = htons(ETH_P_IP);
  arp.hlen = 6;
  arp.plen = 4;
  arp.oper = htons(ARP_REQUEST);
  memcpy(arp.sha, source, 6);
  arp.tpa = htonl(tpa);
  arp.spa = htonl(spa);

  struct arp_packet frame = {0};
    frame.eth = eth;
    frame.arp = arp;

  return frame; 
}
