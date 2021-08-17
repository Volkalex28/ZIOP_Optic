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

    AnP[0].Reg->AI2 = 2300;
    AnP[0].Reg->AI3 = 150;
    AnP[0].Reg->AI12 = 2000;
    AnP[0].Reg->AI1 = 100;
    AnP[0].Reg->Umax1 = 3000;
    AnP[0].Reg->Umin1 = 100;
    AnP[0].Reg->Umin3 = 300;
    PFW->CrashUmaxAnP = 2000;
    PFW->CrashUminAnP = 1000;
    BKIf[0].Reg->R1Plus = 200;
    BKIf[0].Reg->R1Minus = 300;
    BKIf[0].Reg->U1Plus = 400;
    BKIf[0].Reg->U1Minus = 500;

    Screen->ShOT.Ues1 = &AnP[0].Reg->AI2;  //2300;
    Screen->ShOT.Ies1 = &AnP[0].Reg->AI3;   //150;
    Screen->ShOT.Uab  = &AnP[0].Reg->AI12;  //2000;
    Screen->ShOT.Iab  = &AnP[0].Reg->AI1;   //100;

    Screen->ShOT.Riz_p = &BKIf[0].Reg->R1Plus;
    Screen->ShOT.Riz_m = &BKIf[0].Reg->R1Minus;
    Screen->ShOT.Uec_p = &BKIf[0].Reg->U1Plus;
    Screen->ShOT.Uec_m = &BKIf[0].Reg->U1Minus;

    Screen->ShOT.MaxW = &PFW->CrashUmaxAnP;//2000;//
    Screen->ShOT.MinW = &PFW->CrashUminAnP;//1000;//

    if(Screen->ShOT.Setting.EventSh.WindowUstEC1 == true) {
        Offset = 0;

        strcpy(Screen->ShOT.Setting.TitleWindowUst, "±ESI");

        Screen->ShOT.Setting.VisibleSh.UmaxUst = true;
        Screen->ShOT.Setting.VisibleSh.UminUst = true;

        OpenWindow(7, 100, 90);

        Screen->ShOT.Setting.EventSh.WindowUstEC1 = false;
    }
    if(Screen->ShOT.Setting.EventSh.WindowUstAB == true) {
        Offset = 2;

        strcpy(Screen->ShOT.Setting.TitleWindowUst, "ÀÁ");

        Screen->ShOT.Setting.VisibleSh.UmaxUst = false;
        Screen->ShOT.Setting.VisibleSh.UminUst = true;

        OpenWindow(7, 100, 90);

        Screen->ShOT.Setting.EventSh.WindowUstAB = false;
    }
    Screen->ShOT.MaxA = &AnP[0].Reg->Umax1 + Offset*2;
    Screen->ShOT.MinA = &AnP[0].Reg->Umin1 + Offset*2;
 }
