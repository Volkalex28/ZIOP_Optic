/**
 * @file screen.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREEN_SCREEN_H__
#define __SCREEN_SCREEN_H__

#include "confcrash.h"
#include "crash.h"
#include "events.h"
#include "shot.h"
#include "zvu.h"
#include "bki_f.h"
#include "shsn.h"

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

void selectCircle(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);
void selectNormal(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);
void selectNormalBlock(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);

void selectDeviceInCrashAndEvent(void);

#endif // __SCREEN_H__