/**
 * @file exchange.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "lib.h"

#include "mem/panel.h"

typedef void (*vFooV_t)(void);

void ip41(void);
void ip42(void);
void ip43(void);
void ip44(void);

void taskExPanels(uint8_t ip)
{
  vFooV_t fooIP[] = {ip41, ip42, ip43, ip44};

  if(ip < 41 || ip > 44) 
    return;
  else 
    ip -= 41;

  while(true)
  {
    fooIP[ip]();

    Delay(20);
  }
}

void ip41(void)
{

}

void ip42(void)
{

}

void ip43(void)
{

}

void ip44(void)
{
  
}
