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

  Screens->Event.Mode = 0;
  Screens->Event.Config.VisibleButtMode = 0;

  if(Screens->Event.Config.Zero) 
  {
    clearEvents();
    Screens->Event.Config.Zero = 0;
  }

  if(Screens->Event.Mode == 0) 
  {
    Stroka = PFW->N_Event;
    Screens->Event.N_Event = (PFW->CB != 0) ? COUNT_EVENTS : PFW->N_Event;
  }
  
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
    if(PFW->CB == 0) 
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
  cell_first_rr_event.number  = FIRST_RR_EVENT+Screens->Event.Cnt*NUMBER_RR_FOR_ONE_EVENT;
  cell_first_rr_event.ptr     = &CAST_TO_U16(EventBuf);
  reads(cell_first_rr_event, NUMBER_RR_FOR_ONE_EVENT*NUMBER_LINES_ON_SCREEN_EVENT);

  for(i = 0; i < NUMBER_LINES_ON_SCREEN_EVENT; i++) 
  {
    short Number = Screens->Event.Cnt + PFW->CB*COUNT_EVENTS + 1 + i;
    
    if(Number <= Stroka + PFW->CB*COUNT_EVENTS) 
    {
      Screens->Event.EventReg[i].Number = Number;
      if(PFW->CB != 0)
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
    Screens->Event.EventReg[i].Event         = EventBuf[i].Event;  

    Screens->Event.EventReg[i].En = (Screens->Event.EventReg[i].Event != 0);
  }
  Screens->Event.OffsetLine = NEventForLine*41;
}
