/**
 * @file shot.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREENS_SHOT_H__
#define __SCREENS_SHOT_H__

#include "../lib.h"

typedef struct ScreenShot_s 
{
  SCREEN_HEADER(SettScreenShot_s,
    struct VisibleSettScreenShot_s 
    {
      uint16_t _0       : 1;    // 0
      uint16_t UmaxUst  : 1;    // 1
      uint16_t UminUst  : 1;    // 2
    } Visible;                  // 50
    struct EventSettScreenShot_s 
    {
      uint16_t WindowUstEC1	: 1;    // 0
      uint16_t WindowUstAB	: 1;    // 1
    } Event;                    // 51
    char TitleWindowUst[6];     // 52..54
    
    int16_t CrashUmaxAnP;
    int16_t CrashUminAnP;
  );

  int16_t* Ues1;  // 600	
  int16_t* Ies1;	// 601
  int16_t* Uab;	// 602
  int16_t* Iab;	// 603

  int16_t* Uec_p;	// 604
  int16_t* Uec_m;	// 605
  int16_t* Riz_p;	// 606
  int16_t* Riz_m;	// 607
  int16_t* _8;	// 608
  int16_t* _9;	// 609

  int16_t* MaxA; 	// 610
  int16_t* MinA;	// 611
  int16_t* MaxW; 	// 612
  int16_t* MinW;	// 613
} ScreenShot_t;

void screenShot(void);

#endif // __SCREENS_SHOT_H__