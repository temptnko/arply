#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include "arp/sock_setup_arp.h"

int parseResponse(uint8_t *buffer, uint32_t sourceIp, uint32_t targetIp, uint8_t mac){
  struct arp_packet *arp_reply = (struct arp_packet*)buffer;
  if (ntohs(arp_reply->eth.h_proto) != ETH_P_ARP)return -1;
  if (ntohs(arp_reply->arp.oper) != ARP_REPLY)return -1;
  if (ntohl(arp_reply->arp.tpa) != sourceIp)return -1;
  if(ntohl(arp_reply->arp.spa) != targetIp)return -1;

  printf("ARP Reply Received:\n");
  printf("  Sender IP: %s\n", inet_ntoa(*(struct in_addr*)&arp_reply->arp.spa));
  if(mac == 1){
    printf("  Sender MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
      arp_reply->arp.sha[0], arp_reply->arp.sha[1],
      arp_reply->arp.sha[2], arp_reply->arp.sha[3],
      arp_reply->arp.sha[4], arp_reply->arp.sha[5]);
  }
  return 0;
}
