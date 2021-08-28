/**
 * @file crashset.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "crashset.h"
#include "..\alarms.h"
#include "..\screens\screen.h"
#include "..\mem\panel.h"
#include "..\mem\manager.h"


void conversion(void)
{
  switch (Panel->ChooseDevice.Select)
  {
    case 0:  //strcpy(Panel->ChooseDevice.TitleCh, "АРМ");   
            Screens->ConfCrash.Settings.OffsetOfCrash = alPowerOn;
            Screens->ConfCrash.Settings.NumbersCrash = alEndArm - 1;
    break;
    case 1:  //strcpy(Panel->ChooseDevice.TitleCh, "ШОТ");   
            Screens->ConfCrash.Settings.OffsetOfCrash = alEndArm;
            Screens->ConfCrash.Settings.NumbersCrash = alEndShort - alEndArm;
    break;
    case 2:  //strcpy(Panel->ChooseDevice.TitleCh, "ШСН");   
            Screens->ConfCrash.Settings.OffsetOfCrash = alEndShort;
            Screens->ConfCrash.Settings.NumbersCrash = alEndShsn - alEndShort;
    break;
    case 3:  //strcpy(Panel->ChooseDevice.TitleCh, "ШСН-Д"); 
            Screens->ConfCrash.Settings.OffsetOfCrash = alEndShsn;
            Screens->ConfCrash.Settings.NumbersCrash = alAll - alEndShsn;
    break;
    default: //strcpy(Panel->ChooseDevice.TitleCh, " "); 
            Screens->ConfCrash.Settings.OffsetOfCrash = 0;
            Screens->ConfCrash.Settings.NumbersCrash = 0;
    break;
  }
}

void FillAndEditConfigFields (void)
{
  uint16_t i, n, k, off;
  cell_t c; 

  // Screens->ConfCrash.Settings.Maska.State = 0;
  // Screens->ConfCrash.Settings.Maska.MCrash = 0;
  // Screens->ConfCrash.Settings.Maska.MEvent = 0;


  for (k = 0; k < 3; k++)
  {
    int number_alarm = 0;

    for (i = 0; i < 6; i++)
    {
      n = NumberOFCrashes*k;

      // c.type = memPFW; 
      // c.number = FIRST_RR_CONFCRASH + n;// + Screens->ConfCrash.Settings.Offset[0]/16; 
      // reads(c, NumberOFCrashes, &ReadState[n]);  // Вкл/Выкл аварию + Маска аварий + Маска событий

      number_alarm = Screens->ConfCrash.Settings.Offset[0] + i;
      if(ReadState[n + number_alarm/16] & (1 << (number_alarm%16)))
      {
        CAST_TO_U32(Screens->ConfCrash.Settings.Maska) |= (1 << (6*k+i));
      }
      else
      {
        CAST_TO_U32(Screens->ConfCrash.Settings.Maska) &= ~(1 << (6*k+i));
      }
    }
//---------------------------------------------------------------------------------------------------
    number_alarm = Screens->ConfCrash.Settings.Offset[0] + Screens->ConfCrash.Settings.OffsetConf;
    if(ReadState[n + number_alarm/16] & (1 << (number_alarm%16)))
    {
      CAST_TO_U32(Screens->ConfCrash.Settings.Maska) |= (1 << (24 + k));
    }
    else
    {
      CAST_TO_U32(Screens->ConfCrash.Settings.Maska) &= ~(1 << (24 + k));
    }
//---------------------------------------------------------------------------------------------------
    if (CAST_TO_U16(Screens->ConfCrash.Settings.Event) & (1 << (4+k)))//(Screens->ConfCrash.Settings.Event.ChState)
    {
      off = Screens->ConfCrash.Settings.Offset[0]+Screens->ConfCrash.Settings.OffsetConf;
      c.type = memPFW; c.number = off/16; c.value = ReadState[n + c.number];
      if (c.value & (1<<(off%16)))
      {
        c.value &= ~(1<<(off%16));
      }
      else
      {
        c.value |= (1<<(off%16));
      }
      Screens->ConfCrash.Settings._71 = off;
      CAST_TO_U16(Screens->ConfCrash.Settings.Event) &= ~(1 << (4 + k));//Screens->ConfCrash.Settings.Event.ChState = false;
      c.number += FIRST_RR_CONFCRASH + n;
      write(c);
    }
  }
}

void ReadConfigCrashAndEvent(void)
{
  cell_t c; 
  c.type = memPFW; 
  c.number = FIRST_RR_CONFCRASH;// + Screens->ConfCrash.Settings.Offset[0]/16; 
  reads(c, NumberOFCrashes*3, ReadState);  // Вкл/Выкл аварию + Маска аварий + Маска событий

}

void AddCrash(short NumberCrash) 
{
	int i, n;

	if(Alarms[1]->count == COUNT_ALARMS) return;

	for(i = 0; i < Alarms[1]->count; i++) {
		if(Alarms[1]->buf[i] == NumberCrash) {
			return;
		}
	}
	Alarms[1]->buf[Alarms[1]->count] = NumberCrash;
	Alarms[1]->count++;
	addEvent(NumberCrash);
	// Panel->SettPanel.Crash = 0;
	// Panel->SettPanel.KvitCrash = 0;
}

void FillCrash(void) 
{
	int i, n;

  Alarms[2]->count = 0;

  for(i = 0; i < Alarms[1]->count; i++) 
  {
    if (!(ReadState[NumberOFCrashes + (Alarms[1]->buf[i])/16] & (1 << ((Alarms[1]->buf[i])%16))))
      Alarms[2]->buf[Alarms[2]->count++] = Alarms[1]->buf[i];
  }

  for(i = 0; i < Alarms[2]->count; i++)
  {
    Alarms[0]->buf[i] = Alarms[2]->buf[i];
  }
  Alarms[0]->count = Alarms[2]->count;

  for(i = 0; i < Alarms[1]->count; i++) 
  {
    if (!(ReadState[(Alarms[1]->buf[i])/16] & (1 << ((Alarms[1]->buf[i])%16)))) 
    {
      Panel->flags.noneCrash = false;
      return;
    }
  }
  Panel->flags.noneCrash = true;
}


void DeleteCrash(short NumberCrash) {
	int i, n;

	if(!Alarms[1]->count) {
		// Panel->SettPanel.Crash = 1;
		return;
	}
	for(i = 0; i < Alarms[1]->count; i++) {
		if(Alarms[1]->buf[i] == NumberCrash) {
			Alarms[1]->count--;
			for(; i < Alarms[1]->count; i++) {
				Alarms[1]->buf[i] = Alarms[1]->buf[i+1];
			}
			Alarms[1]->buf[i] = 0;

			// if(NumberCrash >= EndEventPanel && NumberCrash < EndEventErrCon) {
			// 	Event(NumberCrash+EndEventErrCon-EndEventPanel);
			// }
		}
	}
}

