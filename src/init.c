/**
 * @file init.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "init.h"
#include "titles.h"

#include "devices/devices.h"

#include "mem/manager.h"
#include "mem/panel.h"
#include "mem/pfw.h"

#include "screens/events.h"

void finit()
{
  cell_t c; c.type = memPFW; c.number = 130;
  c.value = 2000;
  write(c);

  eventClear();
  title_finit();
  devices_finit();
  
  SetUserLevelAvtorisation;
  SetAdminLevelAvtorisation;
}

void init()
{
  mem_init();
  devices_init();
  
  if(PFW->flags.firstTurnOn == true)
  {
    finit();
    PFW->flags.firstTurnOn = false;
  }
  if(getMyIP() == 41)
  {
    Panel->flags.initMaster = true;
  }

  addEvent(alPowerOn);
}
