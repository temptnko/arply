#include "if_setup.h"
#include "sock_setup_Arp"
#include "sock_setup_ioctl.h"

#include <stdint.h>
#include <math.h>

int main(){

  char ssid[64] = {0};

  uint8_t sourceMac[6] = {0};

  uint8_t subMaskBin = 0;
  uint32_t sourceIp = 0;

  int fdArp = settupSocket();
  


  if(getCommandOutput(command, ssid, sizeof(ssid)) == -1)return -1;
  if(getSourceMac(ssid, sourceMac))return -1;
  int retVal = getIfInfo(&subMaskInt, &sourceIp, ssid);
  if(retVal < 0){
    return retVal;
  }
  uint8_t subMaskInt = maskToPrefix(subMasBin);
  if(fdArp == -1)return -1;


  int limit = pow(2, 32 - subMaskInt);
  //uint32_t targetIp = sourceIp & subMaskBin;

  for(int i = 1; i <= limit; i++){
    struct frame = buildFrame(sourceMac, sourceIp, targetIp + i);
  }
  

  return 0;
}
