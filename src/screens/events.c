/**
 * @file events.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "events.h"

#include "screen.h"

#include "../alarms.h"

#include "../mem/manager.h"
#include "../mem/panel.h"
#include "../mem/pfw.h"

void screenEvent(void)
{
  size_t i;
  short Stroka = 0;
  short NEventForLine = 0;
  EventByte_t EventBuf[NUMBER_LINES_ON_SCREEN_EVENT];
  cell_t cell_first_rr_event;
  uint16_t mCB, mNum;

  Screens->Event.Mode = 0;
  Screens->Event.Config.VisibleButtMode = 0;

  if(Screens->Event.Config.Zero) 
  {
    clearEvents(false);
    Screens->Event.Config.Zero = 0;
    Panel->controlFlags.clearEvents = true;
  }

  if(Panel->ChooseDevice.Select == 0) 
  { 
    Stroka = PFW->N_Event; mCB = PFW->CB; mNum = FIRST_RR_EVENT;
    Screens->Event.Text = 0;
    Screens->Event.Config.visibleLoad = true;
  }
  else
  {
    Stroka = PFW->gateSettEvent[Panel->ChooseDevice.Select-1].N_Event;// dMem->Gate->SettEvent[Panel->ChooseDevice.Select-1].N_Event; 
    mCB = PFW->gateSettEvent[Panel->ChooseDevice.Select-1].CB;
    mNum = 40000 + 4000*(Panel->ChooseDevice.Select-1);
    Screens->Event.CounterLoad = Panel->gateEventsState[Panel->ChooseDevice.Select-1].percent;
    Screens->Event.Text = Panel->gateEventsState[Panel->ChooseDevice.Select-1].isWait
      + Panel->gateEventsState[Panel->ChooseDevice.Select-1].isError*2;
    Screens->Event.Config.visibleLoad = Screens->Event.Text != 0 || Screens->Event.CounterLoad == 0;
  }
  Screens->Event.N_Event = (mCB != 0) ? COUNT_EVENTS : Stroka;

  Screens->Event.MaxEvent = COUNT_EVENTS;
  
  if(!Screens->Event.Config.JumpScreen || Screens->Event.Config.Auto) 
  {
    if(Stroka < (NUMBER_LINES_ON_SCREEN_EVENT - 1))
      Screens->Event.Cnt = 0;
    else if(Stroka <= (COUNT_EVENTS - 1)) 
      Screens->Event.Cnt = Stroka - (NUMBER_LINES_ON_SCREEN_EVENT - 1);	
    else 
      Screens->Event.Cnt = Stroka - NUMBER_LINES_ON_SCREEN_EVENT;
  }
  if(!Screens->Event.Config.JumpScreen) 
    Screens->Event.Config.JumpScreen = 1;

  if(Screens->Event.Mode == 0) 
  {
    if(mCB == 0) 
    {
      if(Stroka < (NUMBER_LINES_ON_SCREEN_EVENT - 1))
        Screens->Event.CntMax = 0;	
      else if(Stroka <= (COUNT_EVENTS - 1))
        Screens->Event.CntMax = Stroka - (NUMBER_LINES_ON_SCREEN_EVENT - 1);	
      else
        Screens->Event.CntMax = Stroka - NUMBER_LINES_ON_SCREEN_EVENT;
    }
    else 
    {
      Screens->Event.CntMax = COUNT_EVENTS - NUMBER_LINES_ON_SCREEN_EVENT;
    }
  }
  else {
    Screens->Event.CntMax = COUNT_EVENTS - NUMBER_LINES_ON_SCREEN_EVENT;
  }
  Screens->Event.CntMin = 0;
  
  cell_first_rr_event.type    = memPFW;
  cell_first_rr_event.number  = mNum + Screens->Event.Cnt*NUMBER_RR_FOR_ONE_EVENT;
  cell_first_rr_event.ptr     = &CAST_TO_U16(EventBuf);
  reads(cell_first_rr_event, NUMBER_RR_FOR_ONE_EVENT*NUMBER_LINES_ON_SCREEN_EVENT);

  for(i = 0; i < NUMBER_LINES_ON_SCREEN_EVENT; i++) 
  {
    short Number = Screens->Event.Cnt + mCB*COUNT_EVENTS + 1 + i;
    
    if(Number <= Stroka + mCB*COUNT_EVENTS) 
    {
      Screens->Event.EventReg[i].Number = Number;
      if(mCB != 0)
        NEventForLine++;
    }
    else 
    {
      Screens->Event.EventReg[i].Number = Number - COUNT_EVENTS;
    }

    Screens->Event.EventReg[i].Time.Hour     = EventBuf[i].Hour;
    Screens->Event.EventReg[i].Time.Min      = EventBuf[i].Min;
    Screens->Event.EventReg[i].Time.Sec      = EventBuf[i].Sec;
    Screens->Event.EventReg[i].Time.Day      = EventBuf[i].Day;
    Screens->Event.EventReg[i].Time.Month    = EventBuf[i].Month;
    Screens->Event.EventReg[i].Time.Year     = EventBuf[i].Year;
    if(Panel->ChooseDevice.Select == 0)
      Screens->Event.EventReg[i].Event       = EventBuf[i].Event;  
    else
      Screens->Event.EventReg[i].Event       
        = convertionNumberAlarm(shieldShot+Panel->ChooseDevice.Select-1, EventBuf[i].Event);

    Screens->Event.EventReg[i].En = (Screens->Event.EventReg[i].Event != 0 
      && (Screens->Event.EventReg[i].Number <= Screens->Event.N_Event || mCB != 0)
    );
  }
  Screens->Event.OffsetLine = NEventForLine*41;

  if(Screens->Event.EventReg[4].Event == 0)
    Screens->Event.Config.JumpScreen = false;
}
