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

  clearEvents(true);
  finitDevices();
  finitTitle();
  
  SetUserLevelAvtorisation;
  SetAdminLevelAvtorisation;
}

void init(void)
{
  uint16_t temp[200] = {0};
  cell_t c; c.type = memPFW; c.number = 2000; c.ptr = temp;

  initMem();
  initPanel();
  initPFW();
  initDevices();
  initAlarms();
  initScreens();
  
  Panel->flags.noneCrash = true;
  
  if(PFW->flags.firstTurnOn == true)
  {
    finit();
    PFW->flags.firstTurnOn = false;
  }
  
  if(getMyIP() == 41)
  {
    Panel->flags.initMaster = true;
    Panel->flags.errConMasterOld = true;
    Panel->flags.errConPanel1Old = true;
    Panel->flags.errConPanel2Old = true;
  }

  writes(c, 200);
}

