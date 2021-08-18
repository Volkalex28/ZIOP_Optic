/**
 * @file panel.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __MEM_PANEL_H__
#define __MEM_PANEL_H__

#include "../lib.h"
#include "../alarms.h"

#define NUMBER_EVENTS           1000
#define FIRST_RR_EVENT          5000
#define NUMBER_RR_FOR_ONE_EVENT (sizeof(EventByte_t)/2)

typedef enum EState {stateMaster, stateSlave, stateMasterInit, stateSlaveInit} State_t;

typedef struct Panel_s
{
  struct FlagsPanel_s
  {
    uint32_t initEx       : 1;        // 0
    uint32_t enableEx     : 1;        // 1
    uint32_t menuIsOpen   : 1;        // 2
  } flags;                            // 256
  struct ChooseDevicePanel_s 
  {
    struct EventButChooseDevicePanel_s 
    {
      uint16_t SelectUp   : 1;        // 0
      uint16_t SelectDown : 1;        // 1
    } EventBut;                       // 258
    int16_t Select;                   // 259
    char TitleCh[6];                  // 260..262
  } ChooseDevice;

} Panel_t;

typedef struct Time_s {
  uint16_t Hour;
  uint16_t Min;
  uint16_t Sec;
  uint16_t Day;
  uint16_t Month;
  uint16_t Year;
} Time_t;

typedef struct EventByte_s {
	uint8_t Min;
	uint8_t Hour;
	uint8_t Day;
	uint8_t Sec;
	uint8_t Year;
	uint8_t Month;
	uint16_t Event;
} EventByte_t;

extern Panel_t * Panel;

uint8_t getMyIP(void);
void addEvent(Alarm_t number);

#endif // __MEM_PANEL_H__