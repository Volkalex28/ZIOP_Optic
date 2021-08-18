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

    static short AI2 = 2300;
    static short AI3 = 150;
    static short AI12 = 2000;
    static short AI1 = 100;
    static short Umax1 = 3000;
    static short Umin1 = 100;
    static short Umin3 = 300;
    static short CrashUmaxAnP = 2000;
    static short CrashUminAnP = 1000;
    static short R1Plus = 200;
    static short R1Minus = 300;
    static short U1Plus = 400;
    static short U1Minus = 500;

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

        strcpy(Screens->Shot.Settings.TitleWindowUst, "±ESI");

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
