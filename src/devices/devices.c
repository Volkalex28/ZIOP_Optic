/**
 * @file devices.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "devices.h"

#include "devices_mem.h"

#include "dDP.h"

devices_mem_t * dMem;
devices_pfw_t * dMemPFW;

uint16_t countDevice;
device_t * devices[N_DP+1];

void finitDevices()
{
  finitDP();
}

void initDevices()
{
  countDevice = 0;
  dMem = (devices_mem_t * )&PSW[FIRST_RR_DEV_MEM];

  initDP();
  addDevice(NULL);
}

void addDevice(device_t * ptr)
{
  devices[countDevice++] = ptr;
}
