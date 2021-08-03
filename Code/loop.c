/**
 * @file loop.c
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "lib.h"
#include "init.h"

void fromPFW(void);
void updatePFW(void);

void loop_m(void)
{
  Delay(20);
  init();

  while(TRUE)
  {
    updatePFW();
    Delay(20);
  }
}

void fromPFW(void)
{
  Reads(HMI_LOCAL_MCH, 0, TYPE_PFW, 256, 500, &PSW[2000]);
}

void updatePFW(void)
{
  int i;
  for(i = 0; i < 500; i++)
  {
    uint16_t value;
    Read(HMI_LOCAL_MCH, 0, TYPE_PFW, 256 + i, 0, TYPE_WORD, &value);
    if(value != PSW[2000 + i])
    {
      Write(HMI_LOCAL_MCH, 0, TYPE_PFW, 256 + i, 0, TYPE_WORD, value);
    }
  }
}
