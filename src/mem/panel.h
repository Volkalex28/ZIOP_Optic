/**
 * @file panel.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __MEM_PANEL_H__
#define __MEM_PANEL_H__

#include "manager.h"

#include "../alarms.h"
#include "../lib.h"

#define FIRST_RR_PANEL    256
#define COUNT_RR_PANEL    (500 - FIRST_RR_PANEL)

#define GetUserLevelAvtorisation 	  GetPSBStatus(B_ACCES_2LEVEL)
#define GetAdminLevelAvtorisation 	GetPSBStatus(B_ACCES_9LEVEL)

#define SetUserLevelAvtorisation 	  SetPSB(B_ACCES_2LEVEL)
#define SetAdminLevelAvtorisation 	SetPSB(B_ACCES_9LEVEL)

#define ResetUserLevelAvtorisation 	ResetPSB(B_ACCES_2LEVEL)
#define ResetAdminLevelAvtorisation	ResetPSB(B_ACCES_9LEVEL)

typedef struct controlFlags_s
{
  uint16_t clearEvents      : 1;      // 0
  uint16_t stateTest        : 2;      // 1-2
  uint16_t invertTest       : 1;      // 3
} controlFlags_t;

typedef struct StateEventsGate_s
{
  uint8_t percent;          // 0..7
  uint8_t isWait    : 1;    // 8
  uint8_t isError   : 1;    // 9
} StateEventsGate_t;

typedef struct StateEx_s
{
  uint16_t CounterCorrect;     
  uint16_t CounterNotCorrect;  
  uint16_t CounterErrorTimeout;
  uint16_t CounterError;       
} StateEx_t;

typedef struct StateExOld_s
{
  uint16_t CounterCorrect;   
  uint16_t CounterNotCorrect;
} StateExOld_t;

typedef struct Time_s {
  uint16_t Hour;
  uint16_t Min;
  uint16_t Sec;
  uint16_t Day;
  uint16_t Month;
  uint16_t Year;
} Time_t;

typedef struct Panel_s
{
  struct FlagsPanel_s
  {
    uint32_t initEx               : 1;  // 0
    uint32_t enableEx             : 1;  // 1
    uint32_t menuIsOpen           : 1;  // 2
    uint32_t noneCrash            : 1;  // 3
    uint32_t isMaster             : 1;  // 4
    uint32_t initMaster           : 1;  // 5
    uint32_t errConMaster         : 1;  // 6
    uint32_t errConPanel1         : 1;  // 7
    uint32_t errConPanel2         : 1;  // 8
    uint32_t cvitCrash            : 1;  // 9
    uint32_t isMasterOld          : 1;  // 10
    uint32_t errConMasterOld      : 1;  // 11
    uint32_t errConPanel1Old      : 1;  // 12
    uint32_t errConPanel2Old      : 1;  // 13
    uint32_t errConC1             : 1;  // 14
    uint32_t errConC2             : 1;  // 15
    uint32_t errConC3             : 1;  // 16
    uint32_t errConC4             : 1;  // 17
    uint32_t errConC5             : 1;  // 18
    uint32_t errConC6             : 1;  // 19
    uint32_t StateUserAccessOld   : 1;  // 20
    uint32_t StateAdminAccessOld  : 1;  // 21
    uint32_t chooseTestMode       : 1;  // 22
  } flags;                              // 256-257
  struct ChooseDevicePanel_s  
  {
    struct EventButChooseDevicePanel_s 
    {
      uint16_t SelectUp   : 1;          // 0
      uint16_t SelectDown : 1;          // 1
    } EventBut;                         // 258
    int16_t Select;                     // 259
    char TitleCh[6];                    // 260..262
    uint16_t ResetCrashList;            // 263
  } ChooseDevice;                       // 258..263 
  uint16_t oldScreen;                   // 264
  StateEventsGate_t gateEventsState[3]; // 265..267
  StateEx_t StateExWindow;              // 268..271
  uint16_t NumberDeviceEx;              // 272
  StateEx_t StateEx[6];                 // 273..295
  StateExOld_t StateExOld[6];           // 296..308
  controlFlags_t controlFlags;          // 309
} Panel_t;

extern Panel_t * Panel;

uint8_t getMyIP(void);
Time_t * getTime(void);
void initPanel(void);

#endif // __MEM_PANEL_H__