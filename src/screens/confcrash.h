/**
 * @file confcrash.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREEN_CONFCRASH_H__
#define __SCREEN_CONFCRASH_H__

#include "../lib.h"

typedef struct ScreenConfCrash_s 
{
  SCREEN_HEADER(SettScreenConfCrash_s,
    uint16_t Offset[6];         // 50..55
    struct VisibleSettScreenConfCrash_s 
    {
      uint16_t EnOffset   : 6;    // 0..5
      uint16_t ArrowUp    : 1;    // 6
      uint16_t ArrowDown  : 1;    // 7
      uint16_t ResetCrash : 1;    // 8
    } Visible;                  // 56
    struct EventSettScreenConfCrash_s 
    {
      uint16_t JumpScreen	: 1;    // 0
      uint16_t ResetCrash	: 1;    // 1
      uint16_t Up         : 1;    // 2
      uint16_t Down       : 1;    // 3
    } Event;                    // 57
    int16_t Count;              // 58
    int16_t NCrash;             // 59
    int16_t NumberCrash[6];     // 60..65
    struct {
      uint32_t State  : 6;	// 0
      uint32_t MCrash : 6;  // 1
      uint32_t MEvent : 6;  // 2
      uint32_t OpenWind : 6;  //
      uint32_t ChState  : 1;	// 0
      uint32_t ChMCrash : 1;  // 1
      uint32_t ChMEvent : 1;  // 2
    } Maska;				// 66-67
    uint16_t OffsetConf;    // 68
  );

  int16_t * _1;           // 600
} ScreenConfCrash_t;

void screenConfCrash(void);

#endif // __CONFCRASH_H__