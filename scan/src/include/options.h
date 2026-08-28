#ifndef OPTIONS_H
#define OPTIONS_H


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct {
    uint8_t arp;
    uint8_t recursive;
    uint8_t mac;
    uint8_t output;
    const char *file;
  } options_t;


#ifdef __cplusplus
}
#endif 

#endif
