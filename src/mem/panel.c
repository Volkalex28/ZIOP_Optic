/**
 * @file panel.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "manager.h"
#include "panel.h"
#include "..\mem\crashset.h"

Panel_t * Panel;

uint8_t getMyIP(void) { 
  cell_t c; c.type = memPFW; c.number = 87;
  return read(c).value; 
}

Time_t * time()
{
  PSW[TIME_SEC]    = (PSW[35]/16)*10 + PSW[35]%16;
  PSW[TIME_MIN]    = (PSW[34]/16)*10 + PSW[34]%16;
  PSW[TIME_HOUR]   = (PSW[33]/16)*10 + PSW[33]%16;
  PSW[TIME_DAY]    = (PSW[32]/16)*10 + PSW[32]%16;
  PSW[TIME_MONTH]  = (PSW[31]/16)*10 + PSW[31]%16;
  PSW[TIME_YEAR]   = (LOBYTE(PSW[30])/16)*10 + LOBYTE(PSW[30])%16; 

  return (Time_t *)&PSW[TIME_HOUR];
}

void addEvent(Alarm_t number)
{
  int i;
  EventByte_t	EventS;
  Time_t * pTime;
  cell_t c;

  if (ReadState[NumberOFCrashes*2 + number/16] & (1 << (number%16)))
    return;

  if(PFW->N_Event >= NUMBER_EVENTS) {
    PFW->N_Event = 0;											
    PFW->CB++;
  }
    
  pTime = time();		
  EventS.Sec    = pTime->Sec;
  EventS.Min    = pTime->Min;
  EventS.Hour   = pTime->Hour;
  EventS.Day    = pTime->Day;
  EventS.Month  = pTime->Month;
  EventS.Year   = pTime->Year;
  EventS.Event  = number;

  c.type = memPFW; 
  c.number = FIRST_RR_EVENT + PFW->N_Event*NUMBER_RR_FOR_ONE_EVENT;
  writes(c, NUMBER_RR_FOR_ONE_EVENT, &CAST_TO_U16(EventS));

  PFW->N_Event++;

}
