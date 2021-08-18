/**
 * @file shot.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"

#include "..\alarms.h"

#include "..\mem\panel.h"

void screenShot(void)
{
  static short Offset = 0;

  static short AI2, AI3, AI12, AI1, Umax1, Umin1, Umin3, CrashUmaxAnP;
  static short CrashUminAnP, R1Plus, R1Minus, U1Plus, U1Minus;

  AI2 = 2300;
  AI3 = 150;
  AI12 = 2000;
  AI1 = 100;
  Umax1 = 3000;
  Umin1 = 100;
  Umin3 = 300;
  CrashUmaxAnP = 2000;
  CrashUminAnP = 1000;
  R1Plus = 200;
  R1Minus = 300;
  U1Plus = 400;
  U1Minus = 500;

  Screens->Shot.Ues1 = &AI2;  //2300;
  Screens->Shot.Ies1 = &AI3;   //150;
  Screens->Shot.Uab  = &AI12;  //2000;
  Screens->Shot.Iab  = &AI1;   //100;

  Screens->Shot.Riz_p = &R1Plus;
  Screens->Shot.Riz_m = &R1Minus;
  Screens->Shot.Uec_p = &U1Plus;
  Screens->Shot.Uec_m = &U1Minus;

  Screens->Shot.MaxW = &CrashUmaxAnP;//2000;//
  Screens->Shot.MinW = &CrashUminAnP;//1000;//

  if(Screens->Shot.Settings.Event.WindowUstEC1 == true) {
    Offset = 0;

    strcpy(Screens->Shot.Settings.TitleWindowUst, "ÅÑ1");

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
  Screens->Shot.MaxA = &Umax1 + Offset*2;
  Screens->Shot.MinA = &Umin1 + Offset*2;
 }
