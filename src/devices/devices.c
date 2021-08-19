/**
 * @file devices.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "devices_mem.h"
#include "dDP.h"

devices_mem_t * dMem;
devices_pfw_t * dMemPFW;

void devices_finit()
{
  DP_finit();
}

void devices_init()
{
  DP_init();
}
