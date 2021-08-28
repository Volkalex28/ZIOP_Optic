/**
 * @file dDP.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "dDP.h"

#include "devices.h"
#include "devices_mem.h"

dDP_t DP[N_DP];

void finitDP(void)
{
  size_t i, n;

  for(i = 0; i < N_DP; i++)
  {
    DP[i].instance.main->port           = net0 + i;
    DP[i].instance.main->adress         = 105 + i;
    DP[i].instance.main->flags.enabled  = true;
  }
}

void initDP(void)
{
  size_t i, n;

  for(i = 0; i < N_DP; i++)
  {
    DP[i].instance.main   = &dMemPFW->DP[i];
    DP[i].instance.nreqs  = sizeof(DP[i].reqs)/sizeof(DP[i].reqs[0]);
    for(n = 0; n < DP[i].instance.nreqs; n++)
      DP[i].instance.reqs = &DP[i].reqs[n];
    DP[i].mem             = (dDPmem_t *)&dMem->DP[i];
    
    DP[i].reqs[0].buf     = (uint16_t *)DP[i].mem->regs;
    DP[i].reqs[0].first   = 0;
    DP[i].reqs[0].count   = 4;

    addDevice(&DP[i].instance);
  }
}
