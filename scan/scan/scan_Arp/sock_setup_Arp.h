
#ifndef SOCK_SETUP_H
#define SOCK_SETUP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int settupSocket(void);
struct EthernetFrame buildFrame(const uint8_t source[6], const uint32_t spa, const uint32_t tpa); 

struct EthHeader {
    uint8_t destination[6];
    uint8_t source[6];
    uint16_t type;
};

struct ArpHeader {
    uint16_t htype;
    uint16_t ptype;
 
    uint8_t  hlen;
    uint8_t  plen;

    uint16_t oper;

    uint8_t  sha[6];
    uint32_t  spa;
    uint8_t  tha[6];
    uint32_t  tpa;
};

struct EthernetFrame {
  struct EthHeader eth;
  struct ArpHeader arp;
};

#ifdef __cplusplus
}
#endif

#endif
