/**
 * @file panel.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "panel.h"

#include "manager.h"
#include "pfw.h"

#include "../devices/devices_mem.h"
#include "../screens/screen.h"

Panel_t * Panel;

uint8_t getMyIP(void) { 
  cell_t c; c.type = memPFW; c.number = PFW_IP_ADRESS_4;
  return read(c).value; 
}

Time_t * getTime(void)
{
  PSW[TIME_SEC]    = (PSW[TIME_SEC_HEX]   / 16) * 10 + PSW[TIME_SEC_HEX]   % 16;
  PSW[TIME_MIN]    = (PSW[TIME_MIN_HEX]   / 16) * 10 + PSW[TIME_MIN_HEX]   % 16;
  PSW[TIME_HOUR]   = (PSW[TIME_HOUR_HEX]  / 16) * 10 + PSW[TIME_HOUR_HEX]  % 16;
  PSW[TIME_DAY]    = (PSW[TIME_DAY_HEX]   / 16) * 10 + PSW[TIME_DAY_HEX]   % 16;
  PSW[TIME_MONTH]  = (PSW[TIME_MONTH_HEX] / 16) * 10 + PSW[TIME_MONTH_HEX] % 16;
  PSW[TIME_YEAR]   = (LOBYTE(PSW[TIME_YEAR_HEX]) / 16) * 10 + LOBYTE(PSW[TIME_YEAR_HEX]) % 16; 

  dMem->time = *(Time_t *)&PSW[TIME_HOUR];

  return (Time_t *)&PSW[TIME_HOUR];
}

void initPanel(void)
{
  Panel = (Panel_t *)&PSW[FIRST_RR_PANEL];

  memset(&CAST_TO_U8(Panel->newTime), 0xFF, sizeof(Panel->newTime));
}

void updateTime()
{
  int i, n;
  bool_t write = false;

  switch (dMem->time.Month)  
  {
  case 1:  Panel->maxDay = 31; break;
  case 2:  Panel->maxDay = (dMem->time.Year % 4 == 0) ? 29 : 28; break;
  case 3:  Panel->maxDay = 31; break;
  case 4:  Panel->maxDay = 30; break;
  case 5:  Panel->maxDay = 31; break;
  case 6:  Panel->maxDay = 30; break;
  case 7:  Panel->maxDay = 31; break;
  case 8:  Panel->maxDay = 31; break;
  case 9:  Panel->maxDay = 30; break;
  case 10: Panel->maxDay = 31; break;
  case 11: Panel->maxDay = 30; break;
  case 12: Panel->maxDay = 31; break;
  }

  if(dMem->time.Day > Panel->maxDay) Panel->newTime.Day = Panel->maxDay;

  for(i = 0; i < 6; i++)
  {
    uint16_t * newTime = &CAST_TO_U16(Panel->newTime) + i;

    if(*newTime != 0xFFFF)
    {
      PSW[TIME_BUF_HEX] = ((*newTime / 100) * 256 + (*newTime / 10) * 16 + (*newTime % 10));

      switch (i)
      {
      case 0: PSW[TIME_BUF_SET] |= (1 << 3); break;
      case 1: PSW[TIME_BUF_SET] |= (1 << 4); break;
      case 2: PSW[TIME_BUF_SET] |= (1 << 5); break;
      case 3: PSW[TIME_BUF_SET] |= (1 << 2); break;
      case 4: PSW[TIME_BUF_SET] |= (1 << 1); break;
      case 5: PSW[TIME_BUF_SET] |= (1 << 0); break;
      }

      for(n = 0; PSW[TIME_BUF_SET] != 0 && n < 10; n++) Delay(100);
      
      *newTime = 0xFFFF;
      write = true;
    }
  }

  if(Panel->flags.isMaster == false 
     && Panel->flags2.saveDataTimeSlave == true 
     && Panel->flags2.saveDataTimeMaster == false
     && write)
  {
    Panel->flags2.saveDataTimeSlave = false;
    Panel->flags2.saveDataTime = false;
    return;
  }

  if(Panel->flags.isMaster && write) Panel->flags2.saveDataTime = true;
  else if(write) Panel->flags2.saveDataTimeMaster = true;
}
