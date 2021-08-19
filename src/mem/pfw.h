/**
 * @file pfw.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __PFW_H__
#define __PFW_H__

#include "../lib.h"
#include "../devices/devices_mem.h"

typedef struct PFW_s
{
  struct PFWFlags_s       // 256
  {
    uint32_t firstTurnOn            : 1;  // 0
  } flags;
  uint16_t N_Event;       // 257
  uint16_t CB;            // 258
  // *
  // *
  // *
  devices_pfw_t dMemPFW;  // 
} PFW_t;

extern PFW_t * PFW;

void getPFW();
void updatePFW();

#endif // __PFW_H__