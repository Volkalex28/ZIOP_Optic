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

uint16_t countDevice;
device_t * devices[N_DP+1];

void devices_finit()
{
  DP_finit();
}

void devices_init()
{
  countDevice = 0;
  DP_init();
  addDevice(NULL);
}

void addDevice(device_t * ptr)
{
  devices[countDevice++]  = ptr;
}
