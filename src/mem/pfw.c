/**
 * @file pfw.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "pfw.h"
#include "manager.h"

PFW_t * PFW;

void getPFW()
{
  cell_t c; c.type = memPFW; c.number = FIRST_RR_EEP;
  reads(c, COUNT_RR_EEP, &PSW[FIRST_RR_FOR_EEP]);
}

void updatePFW()
{
  int i;
  for(i = 0; i < COUNT_RR_EEP; i++)
  {
    cell_t c; c.type = memPFW; c.number = FIRST_RR_EEP + i; 
    
    if(read(c).value != PSW[FIRST_RR_FOR_EEP + i])
    {
      c.value = PSW[FIRST_RR_FOR_EEP + i];
      write(c);
    }
  }
}
