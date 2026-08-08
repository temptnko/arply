#include "if_setup.h"
#include "sock_setup_Arp"
#include "sock_setup_ioctl.h"
#include "response_parse.h"

#include <stdint.h>
#include <math.h>

int main(){

  char ssid[64] = {0};

  uint8_t sourceMac[6] = {0};

  uint8_t subMaskBin = 0;
  uint32_t sourceIp = 0;
  struct sockaddr_ll socket_addr = {0}

  int fdArp = settupSocket(ssid, &socket_addr);
  
  if(getCommandOutput(command, ssid, sizeof(ssid)) == -1)return -1;
  if(getSourceMac(ssid, sourceMac))return -1;
  int retVal = getSourceInfo(ssid, sourceMac[6], &sourceIp, &subMaskBin);
  if(retVal < 0){
    return retVal;
  }
  uint8_t subMaskInt = maskToPrefix(subMasBin);
  if(fdArp == -1)return -1;


  int limit = pow(2, 32 - subMaskInt);
  uint32_t targetIp = sourceIp & subMaskBin;


  uint8_t buffer[4096] = 0;
  for(int i = 1; i <= limit; i++){
    struct frame = buildFrame(sourceMac, sourceIp, targetIp + i);
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
