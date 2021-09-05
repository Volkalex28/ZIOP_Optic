
/**
 * @file shsn.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"

#include "..\alarms.h"
#include "..\devices\devices_mem.h"
#include "..\mem\panel.h"

void screenShsn(void)
{
  // static short I10, I11, I12, I13, I2, I3, I4, I5;

  // switch (Screens->Shsn.Settings.Event.Choose)
  // {
  //     case 0: 
  //             I10 = 1;  I11 = 0;  I12 = 1;  I13 = 0;
  //             I2 = 0;   I4 = 1;   I3 = 0;   I5 = 0;
  //     break;
  //     case 1: 
  //             I10 = 0;  I11 = 1;  I12 = 0;  I13 = 1;
  //             I2 = 0;   I4 = 0;   I3 = 1;   I5 = 0;
  //     break;
  // }

  Screens->Shsn.Settings.Event.Vv1_220_inW = dMem->Gate->SHSN[Screens->Shsn.Settings.Event.Choose].DP.DIO.IO.I.I10; // I10;
  Screens->Shsn.Settings.Event.Vv2_220_inW = dMem->Gate->SHSN[Screens->Shsn.Settings.Event.Choose].DP.DIO.IO.I.I11; // I11;
  Screens->Shsn.Settings.Event.Vv1_36_inW  = dMem->Gate->SHSN[Screens->Shsn.Settings.Event.Choose].DP.DIO.IO.I.I12; // I12;
  Screens->Shsn.Settings.Event.Vv2_36_inW  = dMem->Gate->SHSN[Screens->Shsn.Settings.Event.Choose].DP.DIO.IO.I.I13; // I13;

  Screens->Shsn.Settings.Event.Vv1_220_offA = dMem->Gate->SHSN[Screens->Shsn.Settings.Event.Choose].DP.DIO.IO.I.I2; // I2;
  Screens->Shsn.Settings.Event.Vv2_220_offA = dMem->Gate->SHSN[Screens->Shsn.Settings.Event.Choose].DP.DIO.IO.I.I4; // I4;
  Screens->Shsn.Settings.Event.Vv1_36_offA  = dMem->Gate->SHSN[Screens->Shsn.Settings.Event.Choose].DP.DIO.IO.I.I3; // I3;
  Screens->Shsn.Settings.Event.Vv2_36_offA  = dMem->Gate->SHSN[Screens->Shsn.Settings.Event.Choose].DP.DIO.IO.I.I5; // I5;

  if (Screens->Shsn.Settings.Event.Vv1_220_offA)
  {
    strcpy(Screens->Shsn.Settings.TitleVv1_220, "авар. отключен");
  }
  else
  {
    if (Screens->Shsn.Settings.Event.Vv1_220_inW) 
      strcpy(Screens->Shsn.Settings.TitleVv1_220, "в работе");
    else strcpy(Screens->Shsn.Settings.TitleVv1_220, "        ");
  }
  if (Screens->Shsn.Settings.Event.Vv2_220_offA)
  {
    strcpy(Screens->Shsn.Settings.TitleVv2_220, "авар. отключен");
  }
  else
  {
    if (Screens->Shsn.Settings.Event.Vv2_220_inW) 
      strcpy(Screens->Shsn.Settings.TitleVv2_220, "в работе");
    else strcpy(Screens->Shsn.Settings.TitleVv2_220, "        ");
  }
  if (Screens->Shsn.Settings.Event.Vv1_36_offA)
  {
    strcpy(Screens->Shsn.Settings.TitleVv1_36, "авар. отключен");
  }
  else
  {
    if (Screens->Shsn.Settings.Event.Vv1_36_inW) 
      strcpy(Screens->Shsn.Settings.TitleVv1_36, "в работе");
    else strcpy(Screens->Shsn.Settings.TitleVv1_36, "        ");
  }
  if (Screens->Shsn.Settings.Event.Vv2_36_offA)
  {
    strcpy(Screens->Shsn.Settings.TitleVv2_36, "авар. отключен");
  }
  else
  {
    if (Screens->Shsn.Settings.Event.Vv2_36_inW) 
      strcpy(Screens->Shsn.Settings.TitleVv2_36, "в работе");
    else strcpy(Screens->Shsn.Settings.TitleVv2_36, "        ");
  }
  switch (Screens->Shsn.Settings.Event.Choose == 0 ? shieldShsn : shieldShsnD) 
  {
    case shieldShsn:  strcpy(Screens->Shsn.Settings.TitleChoose, "ШСН");    break;
    case shieldShsnD: strcpy(Screens->Shsn.Settings.TitleChoose, "ШСН-Д");  break;
  }
}