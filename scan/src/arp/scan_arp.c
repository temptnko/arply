#include "arp/if_setup.h"
#include "arp/sock_setup_arp.h"
#include "arp/sock_setup_ioctl.h"
#include "options.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <poll.h>
#include <stddef.h>
#include <linux/if_packet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

long long nowMs(void){
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  long long sec = (long long)ts.tv_sec * 1000LL;
  long long ms = ts.tv_nsec / 1000000LL;

  return sec + ms;
}

int scanArp(const options_t *options){
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
    if(ip == sourceIp)continue;
    struct arp_packet frame = buildFrame(sourceMac, sourceIp, ip);
    ssize_t bytesSent = sendto(fdArp, &frame, sizeof(frame), 0, (struct sockaddr*)&socket_addr, sizeof(socket_addr));
    if(bytesSent <= 0){
      perror("sendto failed");
      return 4;
    }
    struct in_addr a = {0};
    a.s_addr = htonl(ip);

    if(options->recursive == 1){
      printf("sent to: %s\n", inet_ntoa(a));
    }
    long long deadline = nowMs() + 500;
    while(1){
      long long timeLeft = deadline - nowMs();
      if(timeLeft <= 0){
        break;
      }

      struct pollfd pfd = {0};
      pfd.fd = fdArp;
      pfd.events = POLLIN;

      int ready = poll(&pfd, 1, (int)timeLeft);
      if(ready < 0){
        if(errno == EINTR){
          continue;
        }else{
          perror("poll failed");
          return 5;
        }
      }
      if(ready == 0)break;
      if(pfd.revents & (POLLERR | POLLHUP | POLLNVAL)){
        fprintf(stderr, "poll reported error");
        return 6;
      }

      if(pfd.revents & POLLIN){
        ssize_t bytesRecv = recvfrom(fdArp, buffer, sizeof(buffer), 0, NULL, NULL);
        if(bytesRecv < 0){
          if(errno == EINTR){
            continue;
          }else{
            perror("recvfrom failed");
            return 7;
          }
        }
        if(parseResponse(buffer, sourceIp, ip, options->mac, options->output, options->file) == -1){
          continue;
        }
        break;
      }
    }
  }
  return 0;
}
