#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include "include/arp/if_setup.h"

int getIfIndex(char *ifName){
  int index = if_nametoindex(ifName);
  if(index == 0){
    perror("if_nametoindex");
    return -1;
  }
  return index;
}

int getCommandOutput(const char *command, char *buf){
  if(!command || !buf){
    return -1;
  }
  buf[0] = '\0'; 
  FILE *cmd = popen(command, "r");
  if(!cmd)return -1;

  if(!fgets(buf, sizeof(buf), cmd)){
    pclose(cmd);
    buf[0] = '\0';
    return -1;
  }
  
  buf[strcspn(buf, "\r\n")] = '\0';

  pclose(cmd);  
  return 0;
}


