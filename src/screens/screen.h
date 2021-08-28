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
  Controllers = 1,
  Mnemotic
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
void clearRRScreens();

/**
 * @brief 
 * 
 */
void fillRRScreens();

void initScreens(void);

#endif // __SCREENS_SCREEN_H__