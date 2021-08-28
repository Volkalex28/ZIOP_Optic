/**
 * @file devices_mem.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __DEVICES_DEVICES_MEM_H__
#define __DEVICES_DEVICES_MEM_H__

#include "dDP.h"

#define FIRST_RR_DEV_MEM 2500

typedef struct devices_mem_s
{
  dDPmem_t DP[N_DP]; 
} devices_mem_t;

typedef struct devices_pfw_s
{
  dmain_t DP[N_DP]; 
} devices_pfw_t;

extern devices_mem_t * dMem;
extern devices_pfw_t * dMemPFW;

#endif // __DEVICES_DEVICES_MEM_H__