/**
 * @file zvu.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREEN_ZVU_H__
#define __SCREEN_ZVU_H__

#include "../lib.h"

typedef struct ScreenZvu_s 
{
  SCREEN_HEADER(SettScreenZvu_s,
    struct VisibleSettScreenZvu_s 
    {
      uint16_t _0       : 1;    // 0
      uint16_t _1       : 1;    // 1
      uint16_t _2       : 1;    // 2
    } Visible;                  // 50
    struct EventSettScreenZvu_s 
    {
      uint16_t StateTcomp	: 1;    // 0
      uint16_t _1	        : 1;    // 1
    } Event;                    // 51
  );

  short* Uout;  	// 500	
  short* Iout;	// 501
  short* Tbat;	// 502
  short* Uzad;	// 503
  short* Izad;	// 504
  short* ModeZVU;		// 505
  short* StateZVU1;	// 506
  short* StateZVU2;	// 507

  short* Uel;		// 508
  short* Kel;		// 509
  short* Koef; 	// 510
  short* Ucalc;	// 511
} ScreenZvu_t;

void screenZvu(void);

#endif // __ZVU_H__

