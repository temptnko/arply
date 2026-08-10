
#ifndef SOCK_SETUP_H
#define SOCK_SETUP_H

#include <stdint.h>
#include <net/ethernet.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARP_REQUEST 1
#define ARP_REPLY 2
#define ARPHRD_ETHER 1

int settupSocket(void);
struct arp_packet buildFrame(const uint8_t source[6], const uint32_t spa, const uint32_t tpa); 
int parseResponse(uint8_t *buffer, uint32_t sourceIp);

/*struct ethhdr {
  uint8_t destination[6];
  uint8_t source[6];
  uint16_t type;
} __attribute__((packed));
*/

struct arphdr {
    uint16_t htype;
    uint16_t ptype;
 
    uint8_t  hlen;
    uint8_t  plen;

    uint16_t oper;
    uint8_t  sha[6];
    uint32_t  spa;
    uint8_t  tha[6];
    uint32_t  tpa;
} __attribute__((packed));

struct arp_packet {
  struct ethhdr eth;
  struct arphdr arp;
} __attribute__((packed));


#ifdef __cplusplus
}
#endif

#endif
