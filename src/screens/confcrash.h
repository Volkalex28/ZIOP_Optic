/**
 * @file confcrash.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREENS_CONFCRASH_H__
#define __SCREENS_CONFCRASH_H__

#include "../lib.h"

#define NUMBER_LINES_ON_SCREEN_CONFCRASH    6

typedef struct ScreenConfCrash_s 
{
  SCREEN_HEADER(SettScreenConfCrash_s,
    uint16_t Offset[NUMBER_LINES_ON_SCREEN_CONFCRASH];        // 50..55
    struct VisibleSettScreenConfCrash_s 
    {
      uint16_t EnOffset   : NUMBER_LINES_ON_SCREEN_CONFCRASH; // 0..5
      uint16_t ArrowUp    : 1;                                // 6
      uint16_t ArrowDown  : 1;                                // 7
      uint16_t ResetCrash : 1;                                // 8
    } Visible;                                                // 56
    struct EventSettScreenConfCrash_s 
    {
      uint16_t JumpScreen	: 1;                                // 0
      uint16_t ResetCrash	: 1;                                // 1
      uint16_t Up         : 1;                                // 2
      uint16_t Down       : 1;                                // 3
      uint16_t ChState    : 1;	                              // 4
      uint16_t ChMCrash   : 1;                                // 5
      uint16_t ChMEvent   : 1;                                // 6
      uint16_t OpenWind   : NUMBER_LINES_ON_SCREEN_CONFCRASH; // 7..12
    } Event;                                                  // 57
    int16_t Count;                                            // 58
    int16_t NCrash;                                           // 59
    int16_t NumberCrash[NUMBER_LINES_ON_SCREEN_CONFCRASH];    // 60..65
    struct MaskaSettScreenConfCrash_s
    {
      uint32_t State    : NUMBER_LINES_ON_SCREEN_CONFCRASH;   // 0..5
      uint32_t MCrash   : NUMBER_LINES_ON_SCREEN_CONFCRASH;   // 6..11
      uint32_t MEvent   : NUMBER_LINES_ON_SCREEN_CONFCRASH;   // 12..17
      uint32_t ChState  : 1;	                                // 18
      uint32_t ChMCrash : 1;                                  // 19
      uint32_t ChMEvent : 1;                                  // 20
    } Maska;				                                          // 66-67
    uint16_t OffsetConf;                                      // 68
    uint16_t OffsetOfCrash;                                   // 69
    uint16_t NumbersCrash;                                    // 70
  );
} ScreenConfCrash_t;

void screenConfCrash(void);

#endif // __SCREENS_CONFCRASH_H__