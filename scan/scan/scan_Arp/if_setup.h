#ifndef IF_SETUP_H
#define IF_SETUP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int getIfInfo(void);//checked: 1
int maskToPrefix(uint32_t subMaskBin);//checked: 1
int getCommandOutput(const char *command, char *buf, size_t bufSize);//checked: 1

#ifdef __cplusplus
}
#endif

#endif
