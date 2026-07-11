
#ifndef SOCK_SETUP_H
#define SOCK_SETUP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int settupSocket(void);//checked: 2
struct EthernetFrame buildFrame(const uint8_t destination[6], const uint8_t source[6], const uint8_t sha[6], const uint8_t spa[4], const uint8_t tpa[4]);//checked: 1 

struct EthernetFrame {
    uint8_t destination[6];
    uint8_t source[6];
    uint16_t type;
    uint16_t htype;
    uint16_t ptype;
    uint8_t  hlen;
    uint8_t  plen;
    uint16_t oper;
    uint8_t  sha[6];
    uint8_t  spa[4];
    uint8_t  tha[6];
    uint8_t  tpa[4];
};//checked: 1

#ifdef __cplusplus
}
#endif

#endif
