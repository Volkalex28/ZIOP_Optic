/**
 * @file shot.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"

#include "..\alarms.h"
#include "..\devices\dGate.h"
#include "..\devices\devices_mem.h"
#include "..\mem\panel.h"

void screenShot(void)
{
  static short Offset = 0;

  Screens->Shot.Settings.CrashUmaxAnP = dMem->Gate->SHOT_CrashUmaxAnP;
  Screens->Shot.Settings.CrashUminAnP = dMem->Gate->SHOT_CrashUminAnP;

  Screens->Shot.Ues1 = &dMem->Gate->SHOT.AnP.AI2;   // &AI2;  //2300;
  Screens->Shot.Ies1 = &dMem->Gate->SHOT.AnP.AI3;   //150;
  Screens->Shot.Uab  = &dMem->Gate->SHOT.AnP.AI12;  //2000;
  Screens->Shot.Iab  = &dMem->Gate->SHOT.AnP.AI1;   //100;

  Screens->Shot.Riz_p = &dMem->Gate->SHOT.BKI.R1Plus;
  Screens->Shot.Riz_m = &dMem->Gate->SHOT.BKI.R1Minus;
  Screens->Shot.Uec_p = &dMem->Gate->SHOT.BKI.U1Plus;
  Screens->Shot.Uec_m = &dMem->Gate->SHOT.BKI.U1Minus;

  Screens->Shot.MaxW = &Screens->Shot.Settings.CrashUmaxAnP;//2000;//
  Screens->Shot.MinW = &Screens->Shot.Settings.CrashUminAnP;//1000;//

  if(Screens->Shot.Settings.Event.WindowUstEC1 == true) {
    Offset = 0;

    strcpy(Screens->Shot.Settings.TitleWindowUst, "ÅSI");

    Screens->Shot.Settings.Visible.UmaxUst = true;
    Screens->Shot.Settings.Visible.UminUst = true;

    OpenWindow(7, 100, 90);

    Screens->Shot.Settings.Event.WindowUstEC1 = false;
  }
  if(Screens->Shot.Settings.Event.WindowUstAB == true) {
    Offset = 2;

    strcpy(Screens->Shot.Settings.TitleWindowUst, "ÀÁ");

    Screens->Shot.Settings.Visible.UmaxUst = false;
    Screens->Shot.Settings.Visible.UminUst = true;

    OpenWindow(7, 100, 90);

    Screens->Shot.Settings.Event.WindowUstAB = false;
  }
  Screens->Shot.MaxA = &dMem->Gate->SHOT.AnP.Umax1 + Offset*2;
  Screens->Shot.MinA = &dMem->Gate->SHOT.AnP.Umin1 + Offset*2;
 }
