/**
 * @file confcrash.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"

#include "..\alarms.h"

#include "..\mem\panel.h"
#include "..\mem\manager.h"

void fillAndEditMaskMessage(void);
void conversion(void);

void screenConfCrash(void)
{
  uint16_t i, n, k, off;

  if(Panel->ChooseDevice.ResetCrashList)
  {
    conversion();
    Screens->ConfCrash.Settings.Count = 0;
    Panel->ChooseDevice.ResetCrashList = 0;
    Screens->ConfCrash.Settings.OffsetConf = 0;
  }
  
  fillAndEditMaskMessage();

  selectNormalBlock(&Screens->ConfCrash.Settings.Count, 
    (Screens->ConfCrash.Settings.NumbersCrash < 6) 
      ? 0 
      : Screens->ConfCrash.Settings.NumbersCrash - 6, 
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
}

void fillAndEditMaskMessage(void)
{
  size_t i, k;

  for (k = 0; k < alarmsMaskCount; k++)
  {
    for(i = 0; i < 6; i++)
    {
      if(isMasked(k, Screens->ConfCrash.Settings.Offset[0] + i))
        CAST_TO_U32(Screens->ConfCrash.Settings.Maska) |= (1 << (6*k + i));
      else
        CAST_TO_U32(Screens->ConfCrash.Settings.Maska) &= ~(1 << (6*k + i));
    }
//-------------------------------------------------------------------------------------------------
    if(isMasked(k, Screens->ConfCrash.Settings.Offset[0] + Screens->ConfCrash.Settings.OffsetConf))
      CAST_TO_U32(Screens->ConfCrash.Settings.Maska) |= (1 << (24 + k));
    else
      CAST_TO_U32(Screens->ConfCrash.Settings.Maska) &= ~(1 << (24 + k));
//-------------------------------------------------------------------------------------------------
    if(CAST_TO_U16(Screens->ConfCrash.Settings.Event) & (1 << (4 + k)))
    {
      uint16_t off;

      off = Screens->ConfCrash.Settings.Offset[0] + Screens->ConfCrash.Settings.OffsetConf;
      setMask(k, off, !isMasked(k, off));
      
      CAST_TO_U16(Screens->ConfCrash.Settings.Event) &= ~(1 << (4 + k));
    }
  }
}

void conversion(void)
{
  switch(Panel->ChooseDevice.Select)
  {
    case 0:   
      Screens->ConfCrash.Settings.OffsetOfCrash = alNone + 1;
      Screens->ConfCrash.Settings.NumbersCrash = alEndArm - 1;
    break;
    case 1:
      Screens->ConfCrash.Settings.OffsetOfCrash = alEndArm;
      Screens->ConfCrash.Settings.NumbersCrash = alEndShort - alEndArm;
    break;
    case 2:
      Screens->ConfCrash.Settings.OffsetOfCrash = alEndShort;
      Screens->ConfCrash.Settings.NumbersCrash = alEndShsn - alEndShort;
    break;
    case 3:
      Screens->ConfCrash.Settings.OffsetOfCrash = alEndShsn;
      Screens->ConfCrash.Settings.NumbersCrash = alAll - alEndShsn;
    break;
    default:
      Screens->ConfCrash.Settings.OffsetOfCrash = 0;
      Screens->ConfCrash.Settings.NumbersCrash = 0;
    break;
  }
}
