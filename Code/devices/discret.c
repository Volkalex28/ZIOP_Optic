/**
 * @file discret.c
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "discret.h"
#include "devices.h"

DP_t DP[MAX_DP];

void init_DP(void) 
{
  int i;
  for(i = 0; i < MAX_DP; i++)
  {
    DP[i].instance = (DeviceEEP_t *)&PSW[2000 + sizeof(DeviceEEP_t)*i/2];

    DP[i].connection.function = f6;
    DP[i].connection.requestSize = 1;
    DP[i].connection.requests = &DP[i].req;

    DP[i].connection.buffSize = sizeof(struct DP_Mem_s)/2;
    DP[i].connection.pBuff = &PSW[2500 + DP[i].connection.buffSize];

    DP[i].mem = (struct DP_Mem_s *)DP[i].connection.pBuff;

    DP[i].req.ptr = DP[i].connection.pBuff;
    DP[i].req.first = 0;
    DP[i].req.count = 4;
  }
}

void finit_DP(void)
{
  int i;
  for(i = 0; i < MAX_DP; i++)
  {
    DP[i].instance->adress = 105;
    DP[i].instance->masterID = 0;
    DP[i].instance->mode = mode_MASTER;
    DP[i].instance->subMode = submode_DP_2020;
    DP[i].instance->port = NET_3;
  }
}
 