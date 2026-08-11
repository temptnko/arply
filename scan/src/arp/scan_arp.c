#include "if_setup.h"
#include "sock_setup_Arp.h"
#include "sock_setup_ioctl.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <linux/if_packet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(){
  const char command[] ="nmcli -t -f DEVICE,TYPE,STATE dev status | awk -F: '$2==\"wifi\" && $3==\"connected\"{print $1; exit}'";

  char ifName[64] = {0};

  uint8_t sourceMac[6] = {0};

  uint32_t subMaskBin = 0;
  uint32_t sourceIp = 0;
  struct sockaddr_ll socket_addr = {0};

  int fdArp = settupSocket();
  
  if(getCommandOutput(command, ifName) == -1)return 2;
  int ifIndex = getIfIndex(ifName);
  if(ifIndex == -1)return 1;
  int retVal = getSourceInfo(ifName, sourceMac, &sourceIp, &subMaskBin);
  if(retVal < 0){
    return retVal;
  }
  if(fdArp == -1)return 3;


  uint32_t network = sourceIp & subMaskBin;
  uint32_t broadcast = network | ~subMaskBin;

  
  socket_addr.sll_family = ARPHRD_ETHER;
  socket_addr.sll_ifindex = ifIndex;
  socket_addr.sll_halen = ETH_ALEN;
  memset(socket_addr.sll_addr, 0xff, 6);

  uint8_t buffer[4096] = {0};
  for(uint32_t ip = network + 1; ip < broadcast; ip++){
    struct arp_packet frame = buildFrame(sourceMac, sourceIp, ip);
    ssize_t bytesSent = sendto(fdArp, &frame, sizeof(frame), 0, (struct sockaddr*)&socket_addr, sizeof(socket_addr));
    if(bytesSent <= 0){
      perror("sendto failed");
      return 4;
    }

    printf("sent to: %u\n", ip);
    
    ssize_t bytesRecv = recvfrom(fdArp, buffer, sizeof(buffer), 0, NULL, NULL);
    if(bytesRecv <= 0){
      perror("recvfrom failed");
      return 5;
    }
    if(parseResponse(buffer, sourceIp) == -1){
      continue;
    }
  }
  return 0;
}
