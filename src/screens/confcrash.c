/**
 * @file confcrash.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"

#include "..\alarms.h"

#include "..\mem\panel.h"
#include "..\mem\crashset.h"
#include "..\mem\manager.h"

void screenConfCrash(void)
{
  uint16_t i, n, k, off;
  cell_t c; 
  

  // ReadState[0]=126;
  // ReadState[NumberOFCrashes]=102;
  // ReadState[NumberOFCrashes*2]=84;

  if (Panel->ChooseDevice.ResetCrashList)
  {
    conversion();
    Screens->ConfCrash.Settings.Count = 0;
    Panel->ChooseDevice.ResetCrashList = 0;
    Screens->ConfCrash.Settings.OffsetConf = 0;
  }
  
  FillAndEditConfigFields();
  // Screens->ConfCrash.Settings.Maska.State = 0;
  // for (i = 0; i < 7; i++)
  // {
  //   k = ReadState [(Screens->ConfCrash.Settings.Offset[0]+i)/16] >> ((Screens->ConfCrash.Settings.Offset[0]+i)%16); 
  //   Screens->ConfCrash.Settings.Maska.State |= k << i;
  // }

  selectNormalBlock(&Screens->ConfCrash.Settings.Count, 
    (Screens->ConfCrash.Settings.NumbersCrash < 6) 
      ? 0 
      : Screens->ConfCrash.Settings.NumbersCrash - 6 , 
    0,
    Screens->ConfCrash.Settings.Event.Up, 
    Screens->ConfCrash.Settings.Event.Down
  );
  Screens->ConfCrash.Settings.Event.Up = Screens->ConfCrash.Settings.Event.Down = false;

  if(Screens->ConfCrash.Settings.Event.JumpScreen == false) 
  {
    Screens->ConfCrash.Settings.Count = 0; 						
    Screens->ConfCrash.Settings.Event.JumpScreen = true; 
  }

  Screens->ConfCrash.Settings.NCrash = Screens->ConfCrash.Settings.NumbersCrash;
  if(Screens->ConfCrash.Settings.NumbersCrash != 0) 
  {
    for(i = 0; i < (Screens->ConfCrash.Settings.NumbersCrash < 6 ? Screens->ConfCrash.Settings.NumbersCrash : 6); i++) 
    {
      Screens->ConfCrash.Settings.NumberCrash[i] = 
        Screens->ConfCrash.Settings.OffsetOfCrash + Screens->ConfCrash.Settings.Count + i;
      Screens->ConfCrash.Settings.Offset[i] = Screens->ConfCrash.Settings.NumberCrash[i];
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

  if(Screens->ConfCrash.Settings.NumbersCrash <= 6) 
  {
    Screens->ConfCrash.Settings.Visible.ArrowUp = Screens->ConfCrash.Settings.Visible.ArrowDown = false;
  }
  else if(Screens->ConfCrash.Settings.Count == 0) 
  {
    Screens->ConfCrash.Settings.Visible.ArrowUp = false;
    Screens->ConfCrash.Settings.Visible.ArrowDown = true;
  }
  else if(Screens->ConfCrash.Settings.Count == (Screens->ConfCrash.Settings.NumbersCrash - 6)) 
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
      break;
   }
  }
  
  // Screens->ConfCrash.Settings.Maska.ChState = ReadState [(Screens->ConfCrash.Settings.Offset[0] + Screens->ConfCrash.Settings.OffsetConf)/16] >> ((Screens->ConfCrash.Settings.Offset[0]+Screens->ConfCrash.Settings.OffsetConf)%16); 
  // Screens->ConfCrash.Settings._71 = Screens->ConfCrash.Settings.Maska.ChState;

  // if (Screens->ConfCrash.Settings.Event.ChState)
  // {
  //   off = Screens->ConfCrash.Settings.Offset[0]+Screens->ConfCrash.Settings.OffsetConf;
  //   c.type = memPFW; c.number = off/16; c.value = ReadState[c.number];
  //   if (c.value & (1<<(off%16)))
  //   {
  //     c.value &= ~(1<<(off%16));
  //   }
  //   else
  //   {
  //     c.value |= (1<<(off%16));
  //   }
  //   Screens->ConfCrash.Settings._71 = off;
  //   Screens->ConfCrash.Settings.Event.ChState = false;
  //   c.number += FIRST_RR_CONFCRASH + NumberOFCrashes*0;
  //   write(c);
  // }
    
    Screens->ConfCrash.Settings._72 = ReadState[Screens->ConfCrash.Settings.Offset[0]/16];
    
    // c.type = memPFW; 
    // c.number = FIRST_RR_CONFCRASH + NumberOFCrashes*0;// + Screens->ConfCrash.Settings.Offset[0]/16; 
    // reads(c, NumberOFCrashes, &ReadState[0]);  // Вкл/Выкл аварию + Маска аварий + Маска событий

    // c.type = memPFW; 
    // c.number = FIRST_RR_CONFCRASH + NumberOFCrashes*1 + Screens->ConfCrash.Settings.Offset[0]/16; 
    // reads(c, 2, &ReadMCrash[Screens->ConfCrash.Settings.Offset[0]/16]);  // Вкл/Выкл аварию + Маска аварий + Маска событий

    // c.type = memPFW; 
    // c.number = FIRST_RR_CONFCRASH + NumberOFCrashes*2 + Screens->ConfCrash.Settings.Offset[0]/16; 
    // reads(c, 2, &ReadMEvent[Screens->ConfCrash.Settings.Offset[0]/16]);  // Вкл/Выкл аварию + Маска аварий + Маска событий

  // for (i == 0; i < 3; i++)
  // {
  //   c.type = memPFW; 
  //   c.number = FIRST_RR_CONFCRASH + NumberOFCrashes*i + Screens->ConfCrash.Settings.Offset[0]/16; 
  //   reads(c, 2, &ReadState[Screens->ConfCrash.Settings.Offset[0]/16]);  // Вкл/Выкл аварию + Маска аварий + Маска событий
  // }
}

