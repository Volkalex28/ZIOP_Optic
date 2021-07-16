/**
 * @file main.cpp
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "main.hpp"
#include "readwrite.hpp"

namespace time
{

enum RR_Time { YEAR = 30, MONTH, DAY, HOURS, MINUTES, SECONDS };

uint8_t convert(RR_Time number)
{
  if(number != RR_Time::YEAR)
  {
    return (RW::psw::read(number)/16)*10 + RW::psw::read(number)%16;
  }
  else
  {
    return ((RW::psw::read(RR_Time::YEAR)&0xFF)/16)*10 
            + (RW::psw::read(RR_Time::YEAR)&0xFF)%16; 
  }
}

time_t get(void) 
{
  time_t time;
  time.Seconds  = convert(RR_Time::SECONDS);
  time.Minutes  = convert(RR_Time::MINUTES);
  time.Hours    = convert(RR_Time::HOURS);
  time.Day      = convert(RR_Time::DAY);
  time.Month    = convert(RR_Time::MONTH);
  time.Year     = convert(RR_Time::YEAR); 

  return time;
}
}