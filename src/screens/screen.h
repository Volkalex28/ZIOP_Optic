/**
 * @file screen.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREENS_SCREEN_H__
#define __SCREENS_SCREEN_H__

#include "confcrash.h"
#include "crash.h"
#include "events.h"
#include "shot.h"
#include "zvu.h"
#include "bki_f.h"
#include "shsn.h"

#define FIRST_RR_SCREEN 500

enum EScreen
{
  scrControllers = 1,
  scrMnemotic,
  scrStateDevices,
  scrInputHandlers,
  scrConfAlarms,
  scrZVU,
  scrBKI,
  scrSHOT,
  scrSHSN,
  scrCrash,
  scrEvent,
  scrSystemSett
};

enum EWindow
{
  winMnemoQ1_7_10 = 1,
  winMnemoQS_T1_T4,
  winMnemoQS_1_4,
  winMnemoQ1_3_6,
  winLevelAccess,
  winSettBKI,
  winSettAnP,
  winConfAlarms,
  winMenuSyst,
  winMenu
};

typedef union Screens_s
{
  ScreenCrash_t     Crash;
  ScreenShot_t      Shot;
  ScreenZvu_t       Zvu;
  ScreenBkif_t      Bkif;
  ScreenShsn_t      Shsn;  
  ScreenConfCrash_t ConfCrash;
  ScreenEvent_t     Event;
} Screens_t;

extern Screens_t * Screens;

void selectDeviceInCrashAndEvent(void);

/**
 * @brief 
 * 
 */
void clearRRScreens(void);

/**
 * @brief 
 * 
 */
void fillRRScreens(void);

/**
 * @brief 
 * 
 */
void updateScreen(void);

/**
 * @brief 
 * 
 */
void controlMenu(void);

/**
 * @brief 
 * 
 */
void initScreens(void);

#endif // __SCREENS_SCREEN_H__