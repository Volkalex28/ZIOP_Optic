/**
 * @file init.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "init.h"
#include "titles.h"

#include "mem/pfw.h"
#include "mem/manager.h"

void finit()
{

}

void init()
{
  mem_init();
  titleInit();
  
  if(PFW->flags.firstTurnOn == true)
  {
    finit();
    PFW->flags.firstTurnOn = false;
  }
}
