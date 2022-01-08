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

#define SHOW_ERROR_MSG(_NUMBER_) Panel->numberTextErrors = (_NUMBER_); OpenWindow(winErrors, 100, 90)

enum numberTextErrors_e
{
  numTE_IncorrectLevelAccess,
  numTE_ErrConectionSHSN,
  numTE_NotConectionSHSN,
  numTE_ErrConectionSHSND,
  numTE_NotConectionSHSND,
  numTE_ErrConectionSHOT,
  numTE_NotConectionSHOT,
  numTE_ErrConectionZVU,
  numTE_NotConectionZVU,
  numTE_ErrConectionBKI,
  numTE_NotConectionBKI,
};

typedef struct controlFlags_s
{
  uint16_t clearEvents      : 1;      // 309:0
  uint16_t stateTest        : 2;      // 309:1..2
  uint16_t invertTest       : 1;      // 309:3
  uint16_t numTestTransform : 2;      // 309:4
} controlFlags_t;

typedef struct StateEventsGate_s
{
  uint8_t percent;          // 265..267:0..7
  uint8_t isWait    : 1;    // 265..267:8
  uint8_t isError   : 1;    // 265..267:9
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
    uint32_t initEx               : 1;  // 256:0
    uint32_t enableEx             : 1;  // 256:1
    uint32_t menuIsOpen           : 1;  // 256:2
    uint32_t noneCrash            : 1;  // 256:3
    uint32_t isMaster             : 1;  // 256:4
    uint32_t initMaster           : 1;  // 256:5
    uint32_t errConMaster         : 1;  // 256:6
    uint32_t errConPanel1         : 1;  // 256:7
    uint32_t errConPanel2         : 1;  // 256:8
    uint32_t cvitCrash            : 1;  // 256:9
    uint32_t isMasterOld          : 1;  // 256:10
    uint32_t errConMasterOld      : 1;  // 256:11
    uint32_t errConPanel1Old      : 1;  // 256:12
    uint32_t errConPanel2Old      : 1;  // 256:13
    uint32_t errConC1             : 1;  // 256:14
    uint32_t errConC2             : 1;  // 256:15
    uint32_t errConC3             : 1;  // 257:0
    uint32_t errConC4             : 1;  // 257:1
    uint32_t errConC5             : 1;  // 257:2
    uint32_t errConC6             : 1;  // 257:3
    uint32_t StateUserAccessOld   : 1;  // 257:4
    uint32_t StateAdminAccessOld  : 1;  // 257:5
    uint32_t chooseTestMode       : 1;  // 257:6
    uint32_t cannotGoToSHSN       : 1;  // 257:7
    uint32_t cannotGoToSHOT       : 1;  // 257:8
    uint32_t cannotGoToZVU        : 1;  // 257:9
    uint32_t cannotGoToBKI        : 1;  // 257:10
    uint32_t notUserAccess        : 1;  // 257:11
    uint32_t notAdminAccess       : 1;  // 257:12
    uint32_t notLevelAccess       : 1;  // 257:13
  } flags;                              // 256..257
  struct ChooseDevicePanel_s  
  {
    struct EventButChooseDevicePanel_s 
    {
      uint16_t SelectUp   : 1;          // 258:0
      uint16_t SelectDown : 1;          // 258:1
    } EventBut;                         // 258
    int16_t   Select;                   // 259
    char      TitleCh[6];               // 260..262
    uint16_t  ResetCrashList;           // 263
  } ChooseDevice;                       // 258..263 
  uint16_t          oldScreen;          // 264
  StateEventsGate_t gateEventsState[3]; // 265..267
  StateEx_t         StateExWindow;      // 268..271
  uint16_t          NumberDeviceEx;     // 272
  StateEx_t         StateEx[6];         // 273..295
  StateExOld_t      StateExOld[6];      // 296..308
  controlFlags_t    controlFlags;       // 309
  uint16_t          numberTextErrors    // 310
} Panel_t;

extern Panel_t * Panel;

uint8_t getMyIP(void);
Time_t * getTime(void);
void initPanel(void);

#endif // __MEM_PANEL_H__