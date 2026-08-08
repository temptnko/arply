#ifndef SOCK_SETUP_IOCTL_H
#define SOCK_SETUP_IOCTL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int getSourceInfo(char *ssid, uint8_t sha[6], uint32_t *sourceIp, uint32_t *subMask);

#ifdef __cplusplus
}
#endif

#endif
