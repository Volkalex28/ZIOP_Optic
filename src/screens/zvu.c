/**
 * @file shot.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"

#include "..\alarms.h"
#include "..\devices\devices_mem.h"
#include "..\mem\panel.h"

void screenZvu(void)
{
  size_t i;

  Screens->Zvu.Settings.Uout        = (dMem->Gate->SHOT.ZVU[0].Uout + dMem->Gate->SHOT.ZVU[1].Uout) / 2;
  Screens->Zvu.Settings.Iout        = dMem->Gate->SHOT.ZVU[0].Iout + dMem->Gate->SHOT.ZVU[1].Iout;// 140 
  Screens->Zvu.Settings.Tbat        = dMem->Gate->SHOT.AnP.TempPVT10;  
  Screens->Zvu.Settings.ModeZVU     = dMem->Gate->SHOT_ZVU_Regime;
  Screens->Zvu.Settings.Uzad[0]     = dMem->Gate->SHOT_ZVU_Uzad[0];
  Screens->Zvu.Settings.Uzad[1]     = dMem->Gate->SHOT_ZVU_Uzad[1];
  Screens->Zvu.Settings.Izad        = dMem->Gate->SHOT.ZVU[0].Imax;
  Screens->Zvu.Settings.Uel         = dMem->Gate->SHOT_Ubanok;
  Screens->Zvu.Settings.Kel         = dMem->Gate->SHOT_ZVU_Nbanok;
  Screens->Zvu.Settings.Koef        = dMem->Gate->SHOT_KTemperature;
  Screens->Zvu.Settings.Ucalc       = dMem->Gate->SHOT_ZVU_UoutTemperature;
  Screens->Zvu.Settings.ConfPanel   = CAST_TO_U16(dMem->Gate->SHOT_ConfPanel);
  Screens->Zvu.Settings.StateZVU[0] = dMem->Gate->SHOT_ZVU_State[0];
  Screens->Zvu.Settings.StateZVU[1] = dMem->Gate->SHOT_ZVU_State[1];

  Screens->Zvu.Uout           = &Screens->Zvu.Settings.Uout;
  Screens->Zvu.Iout           = &Screens->Zvu.Settings.Iout;
  Screens->Zvu.Tbat           = &Screens->Zvu.Settings.Tbat;
  Screens->Zvu.ModeZVU        = &Screens->Zvu.Settings.ModeZVU;
  Screens->Zvu.Uzad           = Screens->Zvu.Settings.ModeZVU == 0 
                                  ? &Screens->Zvu.Settings.Uzad[0] 
                                  : &Screens->Zvu.Settings.Uzad[1];
  Screens->Zvu.Izad           = &Screens->Zvu.Settings.Izad;
  Screens->Zvu.Uel            = &Screens->Zvu.Settings.Uel;
  Screens->Zvu.Kel            = &Screens->Zvu.Settings.Kel;
  Screens->Zvu.Koef           = &Screens->Zvu.Settings.Koef;
  Screens->Zvu.Ucalc          = &Screens->Zvu.Settings.Ucalc;
  Screens->Zvu.ConfPanel      = &Screens->Zvu.Settings.ConfPanel;
  Screens->Zvu.StateZVU[0]    = &Screens->Zvu.Settings.StateZVU[0];
  Screens->Zvu.StateZVU[1]    = &Screens->Zvu.Settings.StateZVU[1];
}