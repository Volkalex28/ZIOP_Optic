/**
 * @file crash.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREENS_CRASH_H__
#define __SCREENS_CRASH_H__

#include "../lib.h"

#define NUMBER_LINES_ON_SCREEN_CRASH    6

typedef struct ScreenCrash_s 
{
  SCREEN_HEADER(SettScreenCrash_s,
    uint16_t Offset[NUMBER_LINES_ON_SCREEN_CRASH];          // 50..55
    struct VisibleSettScreenCrash_s 
    {
      uint16_t EnOffset   : NUMBER_LINES_ON_SCREEN_CRASH;   // 0..5
      uint16_t ArrowUp    : 1;                              // 6
      uint16_t ArrowDown  : 1;                              // 7
      uint16_t ResetCrash : 1;                              // 8
    } Visible;                                              // 56
    struct EventSettScreenCrash_s 
    {
      uint16_t JumpScreen	: 1;                              // 0
      uint16_t ResetCrash	: 1;                              // 1
      uint16_t Up         : 1;                              // 2
      uint16_t Down       : 1;                              // 3
    } Event;                                                // 57
    int16_t Count;                                          // 58
    int16_t NCrash;                                         // 59
    int16_t NumberCrash[NUMBER_LINES_ON_SCREEN_CRASH];      // 60..65
  );
} ScreenCrash_t;

void screenCrash(void);

#endif // __SCREENS_CRASH_H__