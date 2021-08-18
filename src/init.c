/**
 * @file init.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "init.h"
#include "titles.h"

#include "devices/devices.h"

#include "mem/pfw.h"
#include "mem/manager.h"

void finit()
{

}

void init()
{
  mem_init();
  devices_init();
  
  if(PFW->flags.firstTurnOn == true)
  {
    finit();
    title_finit();
    devices_finit();
    PFW->flags.firstTurnOn = false;
  }
}
