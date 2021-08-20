/**
 * @file crash.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "crash.h"
#include "screen.h"

#include "..\alarms.h"

#include "..\mem\panel.h"

void screenCrash(void)
{
  int i, n, k;

  Alarms->count = 14;
  for (i = 1; i <= Alarms->count; i++)
  {
    Alarms->buf[i - 1] = i;
  }
  
  selectCircle(&Screens->Crash.Settings.Count, (Alarms->count < 6) ? 0 : Alarms->count - 6 , 0, \
    Screens->Crash.Settings.Event.Up, Screens->Crash.Settings.Event.Down);
  Screens->Crash.Settings.Event.Up = Screens->Crash.Settings.Event.Down = false;

  if(Screens->Crash.Settings.Event.JumpScreen == false) 
  {
    Screens->Crash.Settings.Count = 0; 						
    Screens->Crash.Settings.Event.JumpScreen = true; 
  }

  Screens->Crash.Settings.NCrash = Alarms->count;
  if(Alarms->count != 0) 
  {
    for(i = 0; i < (Alarms->count < 6 ? Alarms->count : 6); i++) 
    {
      Screens->Crash.Settings.NumberCrash[i] = Alarms->count - Screens->Crash.Settings.Count - i;
      Screens->Crash.Settings.Offset[i] = Alarms->buf[Screens->Crash.Settings.NumberCrash[i] - 1];
      Screens->Crash.Settings.Visible.EnOffset |= (1<<i);
    }
    for(; i < 6; i++) 
    {
      Screens->Crash.Settings.Visible.EnOffset &= ~(1<<i);
    }
  }
  else 
  {
    Screens->Crash.Settings.Event.JumpScreen = false; 
    Screens->Crash.Settings.Visible.EnOffset = false;
  }

  if(Alarms->count <= 6) 
  {
    Screens->Crash.Settings.Visible.ArrowUp = Screens->Crash.Settings.Visible.ArrowDown = false;
  }
  else if(Screens->Crash.Settings.Count == 0) 
  {
    Screens->Crash.Settings.Visible.ArrowUp = false;
    Screens->Crash.Settings.Visible.ArrowDown = true;
  }
  else if(Screens->Crash.Settings.Count == Alarms->count - 6) 
  {
    Screens->Crash.Settings.Visible.ArrowUp = true;
    Screens->Crash.Settings.Visible.ArrowDown = false;
  }
  else 
  {
    Screens->Crash.Settings.Visible.ArrowUp = Screens->Crash.Settings.Visible.ArrowDown = true;
  }

  // if(Panel->SettPanel.KvitCrash == true) {
  //   for(n = 0; n < PFW->N_ZVU; n++) {
  //     if(TO_SHORT(ZVU[n].Reg->Crash) != 0) {
  //       Screens->Crash.Settings.Visible.ResetCrash = true;
  //       break;
  //     }
  //   }
  //   if(n == PFW->N_ZVU)
  //     Screens->Crash.Settings.Visible.ResetCrash = false;
  // }
  // else {
  //   Screens->Crash.Settings.Visible.ResetCrash = false;
  // }
}
