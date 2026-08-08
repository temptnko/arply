#ifndef IF_SETUP_H
#define IF_SETUP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


uint8_t maskToPrefix(uint32_t subMaskBin);
int getCommandOutput(const char *command, char *buf);
int ifIndex(char *ssid);

#ifdef __cplusplus
}
#endif

#endif
