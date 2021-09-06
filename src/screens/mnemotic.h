/**
 * @file mnemotic.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
**/

#ifndef __SCREENS_MNEMOTIC_H__
#define __SCREENS_MNEMOTIC_H__

#include "../lib.h"

typedef struct ScreenMnemotic_s 
{
  SCREEN_HEADER(SettScreenMnemotic_s,
    int16_t TitleWindowNumber;    // 550    
    struct FlagsScreenMnemotic_s 
    {
      uint16_t FlagVisible	: 1;    // 0
      uint16_t Led1	: 1;            // 1
      uint16_t Led2	: 1;            // 2
      uint16_t Led3	: 1;            // 3
      uint16_t EventOpenWindow : 1; // 4
    } Flags;                      // 551
    uint16_t RegimeWindow1;       // 552
    uint16_t RegimeWindow2;       // 553
    
    uint16_t RegimeScreen [28];   // 554...581
    uint16_t Temp;   // 582
  );
} ScreenMnemotic_t;

void screenMnemotic(void);

#endif // __SCREENS_MNEMOTIC_H__