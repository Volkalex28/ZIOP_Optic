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
  cell_t c; c.type = memPFW; c.number = PFW_IP_ADRESS_4;
  return read(c).value; 
}

Time_t * time(void)
{
  PSW[TIME_SEC]    = (PSW[TIME_SEC_HEX]   / 16) * 10 + PSW[TIME_SEC_HEX]   % 16;
  PSW[TIME_MIN]    = (PSW[TIME_MIN_HEX]   / 16) * 10 + PSW[TIME_MIN_HEX]   % 16;
  PSW[TIME_HOUR]   = (PSW[TIME_HOUR_HEX]  / 16) * 10 + PSW[TIME_HOUR_HEX]  % 16;
  PSW[TIME_DAY]    = (PSW[TIME_DAY_HEX]   / 16) * 10 + PSW[TIME_DAY_HEX]   % 16;
  PSW[TIME_MONTH]  = (PSW[TIME_MONTH_HEX] / 16) * 10 + PSW[TIME_MONTH_HEX] % 16;
  PSW[TIME_YEAR]   = (LOBYTE(PSW[TIME_YEAR_HEX]) / 16) * 10 + LOBYTE(PSW[TIME_YEAR_HEX]) % 16; 

  return (Time_t *)&PSW[TIME_HOUR];
}

void initPanel(void)
{
  Panel = (Panel_t *)&PSW[FIRST_RR_PANEL];
}
