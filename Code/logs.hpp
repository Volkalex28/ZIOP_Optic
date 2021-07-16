/**
 * @file logs.hpp
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "main.hpp"
#include "readwrite.hpp"

class logs_t
{
  const uint32_t first;
  const uint16_t numbers;
  const uint16_t rrCurrentNumbers;
  const uint16_t rrNumberOverload;

  struct memEvent_t;

public:
  enum events;
  struct event_t;

  logs_t(const uint32_t first, const uint16_t numbers);
  void operator<<(events number);
  event_t operator>>(events number);

  uint16_t getCurentNumbers(void);
  uint16_t getNumberOverload(void);
};

struct logs_t::event_t
{
  time::time_t time;
  events number;
};

enum logs_t::events
{
  PowerOn,
  OpenUserAccesLevel,
  OpenAdminAccesLevel,
};
