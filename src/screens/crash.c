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
  Alarms_t * currentAlarms = Alarms[alarmsActual + Panel->ChooseDevice.Select];

  if(currentAlarms->count > COUNT_ALARMS) 
  {
    Panel->ChooseDevice.EventBut.SelectUp = true;
    return;
  }

  // if(Panel->flags.isMaster) for (i = 1; i <= 20; i++)
  // {
  //   addCrash(12+i);
  // }
  
  selectCircle(&Screens->Crash.Settings.Count, 
    (currentAlarms->count < NUMBER_LINES_ON_SCREEN_CRASH) 
      ? 0 
      : currentAlarms->count - NUMBER_LINES_ON_SCREEN_CRASH, 
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

  Screens->Crash.Settings.NCrash = currentAlarms->count;
  if(currentAlarms->count != 0) 
  {
    for(i = 0; 
      i < (currentAlarms->count < NUMBER_LINES_ON_SCREEN_CRASH 
        ? currentAlarms->count 
        : NUMBER_LINES_ON_SCREEN_CRASH); 
      i++
    ) {
      Screens->Crash.Settings.NumberCrash[i] = 
        currentAlarms->count - Screens->Crash.Settings.Count - i;
      Screens->Crash.Settings.Offset[i] = (currentAlarms == Alarms[alarmsActual]) 
        ? currentAlarms->buf[Screens->Crash.Settings.NumberCrash[i] - 1] 
        : convertionNumberAlarm(Panel->ChooseDevice.Select, 
            currentAlarms->buf[Screens->Crash.Settings.NumberCrash[i] - 1]
          );
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

  if(currentAlarms->count <= NUMBER_LINES_ON_SCREEN_CRASH) 
  {
    Screens->Crash.Settings.Visible.ArrowUp = Screens->Crash.Settings.Visible.ArrowDown = false;
  }
  else if(Screens->Crash.Settings.Count == 0) 
  {
    Screens->Crash.Settings.Visible.ArrowUp = false;
    Screens->Crash.Settings.Visible.ArrowDown = true;
  }
  else if(Screens->Crash.Settings.Count == currentAlarms->count - 6) 
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
