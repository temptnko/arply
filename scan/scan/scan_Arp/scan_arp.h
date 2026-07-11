
#ifndef SCAN_ARP_H
#define SCAN_ARP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int getWlan0Info(void);//checked: 1
int maskToPrefix(uint32_t subMaskBin);//checked: 1


#ifdef __cplusplus
}
#endif

#endif
