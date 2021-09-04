/**
 * @file zvu.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREENS_ZVU_H__
#define __SCREENS_ZVU_H__

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

    uint16_t Uout;              // 52
    uint16_t Iout;	            // 53
    uint16_t Tbat;	            // 54
    uint16_t Uzad[2];	          // 55-56
    uint16_t Izad;	            // 57
    uint16_t ModeZVU;		        // 58
    uint16_t StateZVU[2];	      // 59-60

    uint16_t Uel;               // 61
    uint16_t Kel;               // 62
    uint16_t Koef;              // 63
    uint16_t Ucalc;             // 64

    uint16_t ConfPanel;         // 65
  );

  uint16_t * Uout;          // 500	
  uint16_t * Iout;          // 501
  uint16_t * Tbat;          // 502
  uint16_t * Uzad;          // 503
  uint16_t * Izad;          // 504
  uint16_t * ModeZVU;       // 505
  uint16_t * StateZVU[2];   // 506-507

  uint16_t * Uel;           // 508
  uint16_t * Kel;           // 509
  uint16_t * Koef;          // 510
  uint16_t * Ucalc;         // 511

  uint16_t * ConfPanel;     // 512
} ScreenZvu_t;

void screenZvu(void);

#endif // __SCREENS_ZVU_H__

