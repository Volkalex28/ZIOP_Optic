/**
 * @file panel.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __MEM_PANEL_H__
#define __MEM_PANEL_H__

#include "../lib.h"

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

extern Panel_t * Panel;

uint8_t getMyIP(void);

#endif // __MEM_PANEL_H__