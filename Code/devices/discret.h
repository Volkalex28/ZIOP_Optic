#ifndef __DISCRET_H__
#define __DISCRET_H__

#include "device.h"

#define MAX_DP 8

typedef struct discret_s
{
  DeviceEEP_t * instance;
  DeviceCon_t connection;
  struct DP_Mem_s
  {
    uint16_t reg[4];
  } * mem;
  Request_t req;
} DP_t;

extern DP_t DP[MAX_DP];

void init_DP(void);
void finit_DP(void);

#endif // __DISCRET_H__