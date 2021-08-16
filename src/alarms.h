/**
 * @file alarms.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __ALARMS_H__
#define __ALARMS_H__

#include "lib.h"

#define COUNT_ALARMS    199

typedef enum EAlarm
{
  alNone,
  alPowerOn,
  alOpenUserAccess,
  alOpenAdminAccess,

  alConFailPanel1,
  alConFailPanel2,
  alConFailPanel3,
  alConFailPanel4,
  alConFailDP1,
  alConFailDP2,
  alConFailDP3,
  alConFailDP4,
  alConFailDP5,
  alConFailDP6,
  alConFailGate,

  
} Alarm_t;

typedef struct Alarms_s
{
  uint16_t count;
  uint16_t buf[COUNT_ALARMS];
} Alarms_t;

extern Alarms_t * Alarms;

#endif // __ALARMS_H__