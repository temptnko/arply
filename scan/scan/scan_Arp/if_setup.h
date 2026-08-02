#ifndef IF_SETUP_H
#define IF_SETUP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


int getIfInfo(uint8_t *subMaskInt, uint32_t *sourceIp, const char *ssid);
int maskToPrefix(uint32_t subMaskBin);
int getCommandOutput(const char *command, char *buf, size_t bufSize);
const char *command ="nmcli -t -f DEVICE,TYPE,STATE dev status | awk -F: '$2==\"wifi\" && $3==\"connected\"{print $1; exit}'";

#ifdef __cplusplus
}
#endif

#endif
