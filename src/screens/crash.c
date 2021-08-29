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

size_t j = 0;

void screenCrash(void)
{
  size_t i, n, k;

  // Alarms[0]->count = 14;
  // if (PSW[256] & 1<<4)
  // {
  //   addCrash(15+(j++));
  //   PSW[256] &= ~(1<<4);
  // }
  for (i = 1; i <= 14; i++)
  {
    addCrash(i);
  }
  
  selectCircle(&Screens->Crash.Settings.Count, 
    (Alarms[alarmsActual]->count < NUMBER_LINES_ON_SCREEN_CRASH) 
      ? 0 
      : Alarms[alarmsActual]->count - NUMBER_LINES_ON_SCREEN_CRASH, 
    0,
    Screens->Crash.Settings.Event.Up, 
    Screens->Crash.Settings.Event.Down
  );
  Screens->Crash.Settings.Event.Up = Screens->Crash.Settings.Event.Down = false;

  if(Screens->Crash.Settings.Event.JumpScreen == false) 
  {
    Screens->Crash.Settings.Count = 0; 						
    Screens->Crash.Settings.Event.JumpScreen = true; 
  }

  Screens->Crash.Settings.NCrash = Alarms[alarmsActual]->count;
  if(Alarms[alarmsActual]->count != 0) 
  {
    for(i = 0; 
      i < (Alarms[alarmsActual]->count < NUMBER_LINES_ON_SCREEN_CRASH 
        ? Alarms[alarmsActual]->count 
        : NUMBER_LINES_ON_SCREEN_CRASH); 
      i++
    ) {
      Screens->Crash.Settings.NumberCrash[i] = 
        Alarms[alarmsActual]->count - Screens->Crash.Settings.Count - i;
      Screens->Crash.Settings.Offset[i] = 
        Alarms[alarmsActual]->buf[Screens->Crash.Settings.NumberCrash[i] - 1];
      Screens->Crash.Settings.Visible.EnOffset |= (1<<i);
    }
    for(; i < NUMBER_LINES_ON_SCREEN_CRASH; i++) 
    {
      Screens->Crash.Settings.Visible.EnOffset &= ~(1<<i);
    }
  }
  else 
  {
    Screens->Crash.Settings.Event.JumpScreen = false; 
    Screens->Crash.Settings.Visible.EnOffset = false;
  }

  if(Alarms[alarmsActual]->count <= NUMBER_LINES_ON_SCREEN_CRASH) 
  {
    Screens->Crash.Settings.Visible.ArrowUp = Screens->Crash.Settings.Visible.ArrowDown = false;
  }
  else if(Screens->Crash.Settings.Count == 0) 
  {
    Screens->Crash.Settings.Visible.ArrowUp = false;
    Screens->Crash.Settings.Visible.ArrowDown = true;
  }
  else if(Screens->Crash.Settings.Count == Alarms[alarmsActual]->count - 6) 
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
