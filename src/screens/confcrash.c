/**
 * @file confcrash.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"

#include "..\alarms.h"

#include "..\mem\panel.h"

void screenConfCrash(void)
{
  int i, n, k;

  Screens->ConfCrash.Settings.Maska.State  = 42;//0b101010;
  Screens->ConfCrash.Settings.Maska.MCrash = 56;//0b111000;
  Screens->ConfCrash.Settings.Maska.MEvent = 51;//0b110011;
  
  Alarms->count = 14;
  for (i = 1; i <= Alarms->count; i++)
  {
    Alarms->buf[i] = i;
  }

  selectDeviceInCrashAndEvent();
  
  selectNormal(&Screens->ConfCrash.Settings.Count, (Alarms->count < 6) ? 0 : Alarms->count - 6 , 0, \
    Screens->ConfCrash.Settings.Event.Up, Screens->ConfCrash.Settings.Event.Down);
  Screens->ConfCrash.Settings.Event.Up = Screens->ConfCrash.Settings.Event.Down = false;

  if(Screens->ConfCrash.Settings.Event.JumpScreen == false) 
  {
    Screens->ConfCrash.Settings.Count = 0; 						
    Screens->ConfCrash.Settings.Event.JumpScreen = true; 
  }

  switch (Panel->ChooseDevice.Select)
  {
    case 0:  strcpy(Panel->ChooseDevice.TitleCh, "юпл");   break;
    case 1:  strcpy(Panel->ChooseDevice.TitleCh, "ьнр");   break;
    case 2:  strcpy(Panel->ChooseDevice.TitleCh, "ьям");   break;
    case 3:  strcpy(Panel->ChooseDevice.TitleCh, "ьям-д"); break;
    default: strcpy(Panel->ChooseDevice.TitleCh, " "); break;
  }

  Screens->ConfCrash.Settings.NCrash = Alarms->count;
  if(Alarms->count != 0) 
  {
    for(i = 0; i < (Alarms->count < 6 ? Alarms->count : 6); i++) 
    {
      Screens->ConfCrash.Settings.NumberCrash[i] = Alarms->count - Screens->ConfCrash.Settings.Count - i;
      Screens->ConfCrash.Settings.Offset[i] = Alarms->buf[Screens->ConfCrash.Settings.NumberCrash[i] - 1];
      Screens->ConfCrash.Settings.Visible.EnOffset |= (1<<i);
    }
    for(; i < 6; i++) 
    {
      Screens->ConfCrash.Settings.Visible.EnOffset &= ~(1<<i);
    }
  }
  else 
  {
    Screens->ConfCrash.Settings.Event.JumpScreen = false; 
    Screens->ConfCrash.Settings.Visible.EnOffset = false;
  }

  if(Alarms->count <= 6) 
  {
    Screens->ConfCrash.Settings.Visible.ArrowUp = Screens->ConfCrash.Settings.Visible.ArrowDown = false;
  }
  else if(Screens->ConfCrash.Settings.Count == 0) 
  {
    Screens->ConfCrash.Settings.Visible.ArrowUp = false;
    Screens->ConfCrash.Settings.Visible.ArrowDown = true;
  }
  else if(Screens->ConfCrash.Settings.Count == Alarms->count - 6) 
  {
    Screens->ConfCrash.Settings.Visible.ArrowUp = true;
    Screens->ConfCrash.Settings.Visible.ArrowDown = false;
  }
  else 
  {
    Screens->ConfCrash.Settings.Visible.ArrowUp = Screens->ConfCrash.Settings.Visible.ArrowDown = true;
  }

  for (i = 0; i < 6; i++)
  {
    if (Screens->ConfCrash.Settings.Maska.OpenWind & (1 << i))
    {
      OpenWindow(8, 100, 64);
      Screens->ConfCrash.Settings.Maska.OpenWind = 0;
      Screens->ConfCrash.Settings.OffsetConf = i;

    }
  }

}
