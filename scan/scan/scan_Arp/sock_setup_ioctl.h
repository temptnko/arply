#ifndef SOCK_SETUP_IOCTL_H
#define SOCK_SETUP_IOCTL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int getSourceMac(char *ssid, uint8_t sha[6]);

#ifdef __cplusplus
}
#endif

#endif
