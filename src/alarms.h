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

  alConRestoredPanel1,
  alConRestoredPanel2,
  alConRestoredPanel3,
  alConRestoredPanel4,
  alConRestoredDP1,
  alConRestoredDP2,
  alConRestoredDP3,
  alConRestoredDP4,
  alConRestoredDP5,
  alConRestoredDP6,
  alConRestoredGate,

  alShotB118_1,
  alShotB118_2,
  alShotAnP_1,
  alShotAnP_2,
  alShotAnP_3,
  alShotAnP_4,
  alShotAnP_5,
  alShotAnP_6,
  alShotAnP_7,
  alShotAnP_8,
  alShotAnP_9,
  alShotAnP_10,
  alShotAnP_11,
  alShotAnP_12,
  alShotAnP_13,
  alShotAnP_14,
  alShotAnP_15,
  alShotAnP_16,
  alShotDP_1,
  alShotDP_2,
  alShotDP_3,
  alShotDP_4,
  alShotDP_5,
  alShotDP_6,
  alShotDP_7,
  alShotDP_8,
  alShotDP_9,
  alShotDP_10,
  alShotDP_11,
  alShotDP_12,
  alShotDP_13,
  alShotDP_14,
  alShotDP_15,
  alShotDP_16,
  alShotDP_17,
  alShotDP_18,
  alShotDP_19,
  alShotDP_20,
  alShotDP_21,
  alShotDP_22,
  alShotDP_23,
  alShotDP_24,
  alShotDP_25,
  alShotDP_26,
  alShotDP_27,
  alShotDP_28,
  alShotDP_29,
  alShotDP_30,
  alShotDP_31,
  alShotDP_32,
  alShotDP_33,
  alShotDP_34,
  alShotDP_35,
  alShotDP_36,
  alShotDP_37,
  alShotDP_38,
  alShotDP_39,
  alShotDP_40,
  alShotBKIf_1,
  alShotBKIf_2,
  alShotBKIf_3,
  alShotBKIf_4,
  alShotBKIf_5,
  alShotBKIf_6,
  alShotBKIf_7,
  alShotBKIf_8,
  alShotBKIf_9,
  alShotBKIf_10,
  alShotBKIf_11,
  alShotBKIf_12,
  alShotBKIf_13,
  alShotBKIf_14,

  alShsn_1,  
  alShsn_2,  
  alShsn_3,  
  alShsn_4,  
  alShsn_5,  
  alShsn_6,  
  alShsn_7,  
  alShsn_8,  
  alShsn_9,  
  alShsn_10,  
  alShsn_11,  
  alShsn_12,  
  alShsn_13,  
  alShsn_14,  
  alShsn_15,  
  alShsn_16,  
  alShsn_17,  
  alShsn_18,  
  alShsn_19,  
  alShsn_20
} Alarm_t;

typedef struct Alarms_s
{
  uint16_t count;
  uint16_t buf[COUNT_ALARMS];
} Alarms_t;

extern Alarms_t * Alarms;

#endif // __ALARMS_H__