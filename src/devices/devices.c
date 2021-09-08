/**
 * @file devices.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "devices.h"

#include "devices_mem.h"

#include "dDP.h"
#include "dGate.h"

devices_mem_t * dMem;
devices_pfw_t * dMemPFW;

uint16_t countDevice;
device_t * devices[N_DP+N_Gate+1];

void finitDevices()
{
  finitDP();
  finitGate();
}

void initDevices()
{
  countDevice = 0;
  dMem = (devices_mem_t * )&PSW[FIRST_RR_DEV_MEM];

  initDP();
  initGate();
  addDevice(NULL);
}

void addDevice(device_t * ptr)
{
  devices[countDevice++] = ptr;
}

bool_t getEnable(uint8_t number)
{
  cell_t c;

  c.type = memPFW; c.number = 2064 + 16*number;
  return read(c).value & 0x2 ? false : true;
}

void setEnable(uint8_t number, bool_t state)
{
  cell_t c;
  bool_t mState;

  mState = getEnable(number);
  
  if(mState == state) return;

  if(!state) c.value = 0x2;
  else c.value = 0;
  
  c.type = memPFW; c.number = 2064 + 16*number;
  write(c);
}
