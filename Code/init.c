/**
 * @file init.c
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "init.h"
#include "devices/discret.h"

void init(void)
{
  init_DP();

  if(PSW[256] == 256)
  {
    finit();
  }
}

void finit(void)
{
  finit_DP();
}
