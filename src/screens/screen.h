/**
 * @file screen.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREEN_SCREEN_H__
#define __SCREEN_SCREEN_H__

#include "crash.h"

enum EScreen
{
  Controllers = 1,
  Mnemotic
};

typedef union Screens_s
{
  ScreenCrash_t Crash;
} Screens_t;

extern Screens_t * Screens;

void selectCircle(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);
void selectNormal(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);

void selectDeviceInCrashAndEvent(void);

#endif // __SCREEN_H__