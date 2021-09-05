/**
 * @file pfw.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __MEM_PFW_H__
#define __MEM_PFW_H__

#include "../lib.h"

#include "../devices/devices_mem.h"

#define FIRST_RR_EEP      256
#define FIRST_RR_FOR_EEP  2000
#define COUNT_RR_EEP      500

typedef struct PFW_s
{
  struct PFWFlags_s       // 256
  {
    uint32_t firstTurnOn      : 1;  // 0
  } flags;
  uint16_t N_Event;       // 257
  uint16_t CB;            // 258
  GateSettEvent_t gateSettEvent[3]; // 259-264
  // Место для других переменных для сохраниения в энергонезависимую память
  devices_pfw_t dMemPFW;  // 
} PFW_t;

extern PFW_t * PFW;

void getPFW(void);
void updatePFW(void);
void initPFW(void);

#endif // __MEM_PFW_H__