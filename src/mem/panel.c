/**
 * @file panel.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "panel.h"

#include "manager.h"
#include "pfw.h"

Panel_t * Panel;

uint8_t getMyIP(void) { 
  cell_t c; c.type = memPFW; c.number = 87;
  return read(c).value; 
}

Time_t * time(void)
{
  PSW[TIME_SEC]    = (PSW[35]/16)*10 + PSW[35]%16;
  PSW[TIME_MIN]    = (PSW[34]/16)*10 + PSW[34]%16;
  PSW[TIME_HOUR]   = (PSW[33]/16)*10 + PSW[33]%16;
  PSW[TIME_DAY]    = (PSW[32]/16)*10 + PSW[32]%16;
  PSW[TIME_MONTH]  = (PSW[31]/16)*10 + PSW[31]%16;
  PSW[TIME_YEAR]   = (LOBYTE(PSW[30])/16)*10 + LOBYTE(PSW[30])%16; 

  return (Time_t *)&PSW[TIME_HOUR];
}

void initPanel(void)
{
  Panel = (Panel_t *)&PSW[FIRST_RR_PANEL];
}
