#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include "if_setup.h"

int getCommandOutput(const char *command, char *buf, size_t bufSize){
  if(!command || !buf || bufSize == 0){
    return -1;
  }
  buf[0] = '\0'; 
  FILE *cmd = popen(command, "r");
  if(!cmd)return -1;

  if(!fgets(buf, bufSize, cmd)){
    pclose(cmd);
    buf[0] = '\0';
    return -1;
  }
  
  buf[strcspn(buf, "\r\n")] = '\0';

  pclose(cmd);  
  return 0;
}

uint8_t maskToPrefix(uint32_t subMaskBin){
  int prefix = 0;
  for(int i = 31;i >= 0;i--){
    uint32_t arrow = 1u << i;
    
    if((subMaskBin & arrow) == 0){
      break;
    }else{
      prefix += 1;
    }
  }
  return prefix;  
}

