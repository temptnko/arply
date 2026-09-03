#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include "arp/sock_setup_arp.h"

int parseResponse(uint8_t *buffer, uint32_t sourceIp, uint32_t targetIp, uint8_t mac, uint8_t output, char *file){
  struct arp_packet *arp_reply = (struct arp_packet*)buffer;
  FILE *stream = stdout;
  int opened = 0;
  if(output == 1 && file != NULL){
    FILE *f = fopen(file, "a");
    if(!f)return -1;
    stream = f;
    opened = 1;
  }
  
  if(ntohs(arp_reply->eth.h_proto) != ETH_P_ARP)return -1;
  if(ntohs(arp_reply->arp.oper) != ARP_REPLY)return -1;
  if(ntohl(arp_reply->arp.tpa) != sourceIp)return -1;
  if(ntohl(arp_reply->arp.spa) != targetIp)return -1;

  fprintf(stream, "####################\n");
  fprintf(stream, "Sender IP: %s\n", inet_ntoa(*(struct in_addr*)&arp_reply->arp.spa));
  if(mac == 1){
    fprintf(stream, "Sender MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
      arp_reply->arp.sha[0], arp_reply->arp.sha[1],
      arp_reply->arp.sha[2], arp_reply->arp.sha[3],
      arp_reply->arp.sha[4], arp_reply->arp.sha[5]);
  }
  fprintf(stream, "####################\n\n");
  fflush(stream);
  if(opened == 1){
    fclose(stream);
  }
  return 0;
};
