/**
 * @file devices_mem.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __DEVICES_DEVICES_MEM_H__
#define __DEVICES_DEVICES_MEM_H__

#include "dDP.h"
#include "dGate.h"
#include "../mem/panel.h"


#define FIRST_RR_DEV_MEM 2500

typedef struct devices_mem_s
{
  dDPmem_t    DP[N_DP];         // 2500..2523
  uint16_t    handledDP[16];    // 2524..2539
  Time_t      time;             // 2540..2545
  TimeByte_t  setTime[2];       // 2546..2599
  uint16_t    reserve[48];      // 2546..2599
  dGatemem_t  Gate[N_Gate];     // 2600...
} devices_mem_t;

typedef struct devices_pfw_s
{
  dmain_t DP[N_DP]; 
  dmain_t Gate[N_Gate];
} devices_pfw_t;

extern devices_mem_t * dMem;
extern devices_pfw_t * dMemPFW;

#endif // __DEVICES_DEVICES_MEM_H__