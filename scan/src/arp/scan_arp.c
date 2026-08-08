#include "if_setup.h"
#include "sock_setup_Arp.h"
#include "sock_setup_ioctl.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <linux/if_packet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(){
  const char *command ="nmcli -t -f DEVICE,TYPE,STATE dev status | awk -F: '$2==\"wifi\" && $3==\"connected\"{print $1; exit}'";

  char ssid[64] = {0};

  uint8_t sourceMac[6] = {0};

  uint32_t subMaskBin = 0;
  uint32_t sourceIp = 0;
  struct sockaddr_ll socket_addr = {0};

  int fdArp = settupSocket();
  
  if(getCommandOutput(command, ssid) == -1)return -1;
  int retVal = getSourceInfo(ssid, sourceMac, &sourceIp, &subMaskBin);
  if(retVal < 0){
    return retVal;
  }
  uint8_t subMaskInt = maskToPrefix(subMaskBin);
  if(fdArp == -1)return -1;


  int limit = pow(2, 32 - subMaskInt);
  uint32_t targetIp = sourceIp & subMaskBin;


  uint8_t buffer[4096] = {0};
  for(int i = 1; i <= limit; i++){
    struct arp_packet frame = buildFrame(sourceMac, sourceIp, targetIp + i);
    ssize_t bytesSent = sendto(fdArp, &frame, sizeof(frame), 0, (struct sockaddr*)&socket_addr, sizeof(socket_addr));
    if(bytesSent <= 0){
      perror("sendto failed");
      return -1;
    }
    ssize_t bytesRecv = recvfrom(fdArp, buffer, sizeof(buffer), 0, NULL, NULL);
    if(bytesRecv <= 0){
      perror("recvfrom failed");
      return -1;
    }
    if(parseResponse(buffer, sourceIp) == -1){
      return -1;
    }
  }
  return 0;
}
