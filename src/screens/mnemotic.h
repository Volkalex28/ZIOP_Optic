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
      uint16_t Led1Visible	: 1;    // 5
      uint16_t Led2Visible	: 1;    // 6
      uint16_t Led3Visible	: 1;    // 7
      uint16_t VisibleTemperT1 : 1; // 8
      uint16_t VisibleTemperT2 : 1; // 9
      uint16_t VisibleTemperT3 : 1; // 10
      uint16_t VisibleTemperT4 : 1; // 11

    } Flags;                      // 551
    uint16_t RegimeWindow1;       // 552
    uint16_t RegimeWindow2;       // 553
    
    uint16_t RegimeScreen [28];   // 554...581
    uint16_t Temp;   // 582
    uint16_t TitleWindowNumberDouble;    // 583    

  );
} ScreenMnemotic_t;

//                                    0     1     2     3     4     5     6     7     8
const uint16_t MaskaTrans[2][9]  = {{0x1B, 0x13, 0x1B, 0x1B, 0x13, 0x12, 0x1A, 0x1B, 0x1A},
                                     0x0F, 0x0B, 0x0F, 0x0F, 0x0B, 0x0A, 0x0E, 0x0F, 0x0E};
//                                    0     1     2     3     4     5     6     7     8
const uint16_t ValueTrans[2][9]  = {{0x08, 0x10, 0x00, 0x01, 0x11, 0x12, 0x0A, 0x09, 0x02},
                                    {0x01, 0x09, 0x05, 0x04, 0x08, 0x0A, 0x02, 0x00, 0x06}};
typedef enum KnifesType
{
  Red,
  Green,
  Yellow
};

void screenMnemotic(void);
// uint16_t GroundingKnife (uint16_t reg, uint16_t offset);
// uint16_t WithdrawableCircuitBreaker (uint16_t reg, uint16_t regime, uint16_t offset); 

#endif // __SCREENS_MNEMOTIC_H__