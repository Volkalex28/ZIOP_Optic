/**
 * @file logs.cpp
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "logs.hpp"

struct logs_t::memEvent_t {
	uint8_t Min;
	uint8_t Hour;
	uint8_t Day;
	uint8_t Sec;
	uint8_t Year;
	uint8_t Month;
	uint16_t Event;
};

logs_t::logs_t(const uint32_t first, const uint16_t numbers) 
  : first(first), numbers(numbers), rrCurrentNumbers(first+numbers), rrNumberOverload(rrCurrentNumbers+1)
{ /* Nothing */ }

void logs_t::operator<<(events number) 
{
  using namespace RW::pfw;
  memEvent_t memEvent;
    
	if(read(this->rrCurrentNumbers) >= this->numbers) {
    write(this->rrCurrentNumbers, 0);
    write(this->rrNumberOverload, read(this->rrNumberOverload) + 1);
  }
  
  time::time_t time = time::get();
	memEvent.Sec 		= time.Seconds;
	memEvent.Min 		= time.Minutes;
	memEvent.Hour		= time.Hours;
	memEvent.Day 		= time.Day;
	memEvent.Month 	= time.Month;
	memEvent.Year 	= time.Year;
	memEvent.Event	= number;

  write(this->rrCurrentNumbers + this->rrCurrentNumbers*sizeof(memEvent_t)/2, sizeof(memEvent_t)/2, &TO_UINT16(memEvent));
  write(this->rrCurrentNumbers, read(this->rrCurrentNumbers) + 1);
}

logs_t::event_t logs_t::operator>>(events number) 
{
  using namespace RW::pfw;
  event_t event;
  memEvent_t memEvent;

  read(this->rrCurrentNumbers + number*sizeof(memEvent_t)/2, sizeof(memEvent_t)/2, &TO_UINT16(memEvent));
  event.time.Seconds  = memEvent.Sec;
  event.time.Minutes  = memEvent.Min;
  event.time.Hours    = memEvent.Hour;
  event.time.Day      = memEvent.Day;
  event.time.Month    = memEvent.Month;
  event.time.Hours    = memEvent.Hour;
  event.number        = (events)memEvent.Event;

  return event;
}

uint16_t logs_t::getCurentNumbers(void) 
{
  return this->rrCurrentNumbers;
}

uint16_t logs_t::getNumberOverload(void) 
{
  return this->rrNumberOverload;
}
