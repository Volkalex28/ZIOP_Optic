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

  Screens->Zvu.Settings.Uout    = (dMem->Gate->SHOT.ZVU[0].Uout + dMem->Gate->SHOT.ZVU[1].Uout) / 2;
  Screens->Zvu.Settings.Iout    = dMem->Gate->SHOT.ZVU[0].Iout + dMem->Gate->SHOT.ZVU[1].Iout;// 140 
  Screens->Zvu.Settings.Tbat    = 2900;  
  Screens->Zvu.Settings.ModeZVU = 0;
  Screens->Zvu.Settings.Uzad    = 2420;
  Screens->Zvu.Settings.Izad    = dMem->Gate->SHOT.ZVU[0].Imax;
  Screens->Zvu.Settings.Uel     = 245;
  Screens->Zvu.Settings.Kel     = 50;
  Screens->Zvu.Settings.Koef    = 123;
  Screens->Zvu.Settings.Ucalc   = 2200;

  for(i = 0; i < 2; i++)
  {
    Screens->Zvu.StateZVU[i] = &Screens->Zvu.Settings.StateZVU[i];
    if(dMem->Gate->SHOT.ZVU[i].State.Crash == true || dMem->Gate->SHOT.ZVU[i].State.Protect == true)
      Screens->Zvu.Settings.StateZVU[i] = 2;
    else if(dMem->Gate->SHOT.ZVU[i].State.State == 0)
      Screens->Zvu.Settings.StateZVU[i] = 1;
    else
      Screens->Zvu.Settings.StateZVU[i] = 0;
  }

  Screens->Zvu.Uout     = &Screens->Zvu.Settings.Uout;
  Screens->Zvu.Iout     = &Screens->Zvu.Settings.Iout;
  Screens->Zvu.Tbat     = &Screens->Zvu.Settings.Tbat;
  Screens->Zvu.Uzad     = &Screens->Zvu.Settings.Uzad;
  Screens->Zvu.Izad     = &Screens->Zvu.Settings.Izad;
  Screens->Zvu.Uel      = &Screens->Zvu.Settings.Uel;
  Screens->Zvu.Kel      = &Screens->Zvu.Settings.Kel;
  Screens->Zvu.Koef     = &Screens->Zvu.Settings.Koef;
  Screens->Zvu.Ucalc    = &Screens->Zvu.Settings.Ucalc;
  Screens->Zvu.Settings.Event.StateTcomp = 1;
}