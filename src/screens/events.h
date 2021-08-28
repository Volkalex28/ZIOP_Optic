/**
 * @file events.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREENS_EVENTS_H__
#define __SCREENS_EVENTS_H__

#include "../lib.h"
#include "../mem/panel.h"

#define VERS_EVENT                2
#define NUMBER_LINES_ON_SCREEN    6

typedef struct ScreenEvent_s 
{
  struct ScreenEventEvent_s {
    Time_t Time;
    uint16_t Number;
    uint16_t Event;
    uint16_t Empty;
    uint16_t En;
  } EventReg[NUMBER_LINES_ON_SCREEN];     //0-59
  int16_t CntMax;         //60
  int16_t CntMin;         //61
  struct EventConfig {
    unsigned short JumpScreen       : 1;  // 0
    unsigned short Auto             : 1;  // 1
    unsigned short VisibleButtMode  : 1;  // 2
    unsigned short Zero             : 1;  // 3
  } Config;               //62
  uint16_t Mode;          //63
  int16_t Cnt;            //64
  int16_t OffsetLine;     //65
  uint16_t N_Event;       //66
  uint16_t MaxEvent;      //67
} ScreenEvent_t;

void screenEvent(void);

#endif // __SCREENS_EVENTS_H__
