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
#include "mnemotic.h"

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
  winReboot = 1,
  winTime,
  winErrors = 5,
  winSettBKI,
  winSettAnP,
  winConfAlarms,
  winMenuSyst,
  winMenu,
  winExControl,
  winTCPStatistic,
  winMnemotic
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
  ScreenMnemotic_t  Mnemotic;
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