/**
 * @file exchange.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "lib.h"

#include "devices/devices.h"
#include "mem/panel.h"

//-------------------------
void writeDevice();
void readDevice();

void taskExchangeRead(void)
{
  while(true)
  {
    readDevice();
    Delay(20);
  }
}

void taskExchangeReadGate(void)
{
  while(true)
  {
    cell_t c;

    Delay(20);
    if(Panel->flags.isMaster == false)
      continue;

    c.type = net2; c.number = 71;
    reads(c, 3, PSW + 2600);
  }
}

void taskExchangeWrite(void)
{
  while(true)
  {
    writeDevice();

    Delay(20);
  }
}

void writeDeviceMaster(MemTypes_t port, uint8_t number)
{
  int i;
  for(i = 0; i < 2; i++)
  {
    if(PSW[300] & (1 << (number + 8*i)))
    {
      cell_t c;
      c.type = port; c.adress = 1; c.number = 1000 + 2*number + i; c.value = PSW[c.number];
      if(write(c).status == memStatusOK)
      { 
        c.number = 900; c.value = 1 << (number + i*8);
        if(write(c).status == memStatusOK)
          PSW[300] &= ~(1 << (number + 8*i));
      }
    }
  }
}

void writeDeviceSlave(MemTypes_t port, uint8_t number)
{
  int i;
  for(i = 0; i < 2; i++)
  {
    if(PSW[900] & (1 << (number + 8*i)))
    {
      cell_t c;
      c.type = port; c.adress = 1; c.number = 2 + i; c.value = PSW[1000+2*number+i];
      if(write(c).status == memStatusOK)
        PSW[900] &= ~(1 << (number + 8*i));
    }
  }
}

void writeDevice()
{
  int i;
  switch (getMyIP())
  {
  case 41:
  case 42:
    // writeDeviceMaster(net0, 0);
    // writeDeviceMaster(net1, 1);
    // writeDeviceMaster(net0, 2);
    // writeDeviceMaster(net0, 3);
    // writeDeviceMaster(net1, 4);
    // writeDeviceMaster(net1, 5);
    break;
  case 43: 
    writeDeviceSlave(net0, 0); 
    writeDeviceSlave(net1, 2); 
    writeDeviceSlave(net2, 3); 
    break;
  case 44: 
    writeDeviceSlave(net0, 1); 
    writeDeviceSlave(net1, 4); 
    writeDeviceSlave(net2, 5); 
    break;
  
  default:
    break;
  }
}

void readDevice()
{
  int i;
  struct FlagsPanel_s flags;
  cell_t c;

  c.adress = 1;

  switch (getMyIP())
  {
  case 41: 
  case 42: 

    c.type = net3; c.number = FIRST_RR_PANEL + (&CAST_TO_U16(Panel->flags) - &CAST_TO_U16(*Panel));
    Panel->flags.errConMaster = reads(c, sizeof(flags)/2, &CAST_TO_U16(flags)).status != memStatusOK ? true : false;
    
    c.type = net0; c.number = 2500;
    Panel->flags.errConPanel1 = reads(c, 4, PSW + 2500).status != memStatusOK ? true : false;
    if(Panel->flags.errConPanel1 == false)
    {
      c.number = 2508;
      Panel->flags.errConPanel1 = reads(c, 8, PSW + 2508).status != memStatusOK ? true : false;
    }

    c.type = net1; c.number = 2504;
    Panel->flags.errConPanel2 = reads(c, 4, PSW + 2504).status != memStatusOK ? true : false;
    if(Panel->flags.errConPanel2 == false)
    {
      c.number = 2516;
      Panel->flags.errConPanel2 = reads(c, 8, PSW + 2516).status != memStatusOK ? true : false;
    }

    if(Panel->flags.errConMaster && Panel->flags.errConPanel1 && Panel->flags.errConPanel2)
      Panel->flags.isMaster = false;
    if(Panel->flags.isMaster && flags.isMaster && flags.initMaster == false)
      Panel->flags.isMaster = false;
    if(Panel->flags.errConPanel1 == false || Panel->flags.errConPanel2 == false)
      if(Panel->flags.errConMaster == true || (Panel->flags.errConMaster == false && flags.initMaster == false && flags.isMaster == false))
        Panel->flags.isMaster = true;

    break;

  case 43:
    c.type = net3; c.number = 2504;
    reads(c, 4, PSW + 2504);
    c.number = 2516;
    reads(c, 8, PSW + 2516);
    break;
    
  case 44:
    c.type = net3; c.number = 2500;
    reads(c, 4, PSW + 2500);
    c.number = 2508;
    reads(c, 8, PSW + 2508);
    break;
  
  default:
    break;
  }
}
