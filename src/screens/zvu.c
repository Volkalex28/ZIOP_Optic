/**
 * @file shot.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"

#include "..\alarms.h"

#include "..\mem\panel.h"

void screenZvu(void)
{
  static short Uout, Iout, TemperKomp, Regime, Uzad, Izad;
  static short Ubanok, ZVU_Nbanok, Ktemp, ZVU_UoutTemp;

  Uout = 2400;    
  Iout = 140;    
  TemperKomp = 2900;  
  Regime = 0;
  Uzad = 2420;
  Izad = 1000;
  Ubanok = 245;
  ZVU_Nbanok = 50;
  Ktemp = 123;
  ZVU_UoutTemp = 2200;

  Screens->Zvu.Uout = &Uout;  	// 500	
  Screens->Zvu.Iout = &Iout;	// 501
  Screens->Zvu.Tbat = &TemperKomp;	// 502
  Screens->Zvu.Uzad = &Uzad;	// 503
  Screens->Zvu.Izad = &Izad;	// 504
  Screens->Zvu.Uel = &Ubanok;		// 508
  Screens->Zvu.Kel = &ZVU_Nbanok;		// 509
  Screens->Zvu.Koef = &Ktemp; 	// 510
  Screens->Zvu.Ucalc = &ZVU_UoutTemp;	// 511
  Screens->Zvu.Settings.Event.StateTcomp = 1;
}