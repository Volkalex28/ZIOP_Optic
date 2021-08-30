/**
 * @file init.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "init.h"

#include "alarms.h"
#include "titles.h"

#include "devices/devices.h"

#include "mem/manager.h"
#include "mem/panel.h"
#include "mem/pfw.h"

#include "screens/screen.h"

void finit(void)
{
  cell_t c; c.type = memPFW; c.number = 130;
  c.value = 2000;
  write(c);

  clearEvents();
  finitDevices();
  finitTitle();
  
  SetUserLevelAvtorisation;
  SetAdminLevelAvtorisation;
}

void init(void)
{
  initMem();
  initPanel();
  initPFW();
  initDevices();
  initAlarms();
  initScreens();
  
  if(PFW->flags.firstTurnOn == true)
  {
    finit();
    PFW->flags.firstTurnOn = false;
  }
  
  if(getMyIP() == 41)
  {
    Panel->flags.initMaster = true;
  }
}

