#ifndef IF_SETUP_H
#define IF_SETUP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


int getCommandOutput(const char *command, char *buf);
int getIfIndex(char *ifName);

#ifdef __cplusplus
}
#endif

#endif
