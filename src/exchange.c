/**
 * @file exchange.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "lib.h"

#include "devices/devices.h"
#include "devices/devices_mem.h"

#include "mem/panel.h"

//-------------------------
void writeDevice(void);
void readDevice(void);


void taskExchangeRead(void)
{
  while(true)
  {
    Delay(20);
    if(Panel->flags.enableEx == false) continue;

    readDevice();
  }
}

void taskExchangeReadGate(void)
{
  while(true)
  {
    cell_t c, c_alarm;
    size_t i;

    Delay(300);
    if(Panel->flags.enableEx == false) continue;

    if(Panel->flags.isMaster == false)
    {
      if(Panel->flags.errConMaster == true) continue;

      c.type = net3; c.number = &CAST_TO_U16(dMem->Gate) - PSW; c.ptr = &CAST_TO_U16(dMem->Gate);
      if(reads(c, 42).status != memStatusOK) continue;
      c.number += 42; c.ptr += 42;
      if(reads(c, 112).status != memStatusOK) continue;

      c.number = 1200; c.ptr = PSW + 1200;
      if(reads(c, 100).status != memStatusOK) continue;
      c.number += 100; c.ptr += 100;
      if(reads(c, 100).status != memStatusOK) continue;
    }
    else
    {
      c.type = net2;
      c.type = net2; c.number = 71; c.ptr = PSW + 3000;
      if(reads(c, 3).status != memStatusOK) continue;

      c.number = 120; c.ptr = &CAST_TO_U16(dMem->Gate);
      if(reads(c, 42).status != memStatusOK) continue;
      c.number = 162; c.ptr = &CAST_TO_U16(dMem->Gate) + 42;
      if(reads(c, 112).status != memStatusOK) continue;

      for(i = 0; i < 1; i++)
      {
        c.number = 49; c.value = 0x11 + i*2;
        if(write(c).status != memStatusOK) continue;

        c_alarm.value = 0;
        while(!(c_alarm.value & 0x8000))
        {
          c_alarm = read(c);
          if(c_alarm.status != memStatusOK) break;
          Delay(1000);
        }

        c.number = 300; c.ptr = PSW + 1200 + 200*i;
        if(reads(c, 50).status != memStatusOK) continue;
        c.number += 50; c.ptr += 50;
        if(reads(c, 50).status != memStatusOK) continue;
        c.number += 50; c.ptr += 50;
        if(reads(c, 50).status != memStatusOK) continue;
        c.number += 50; c.ptr += 50;
        if(reads(c, 0).status != memStatusOK) continue;
      }
    }
  }
}

void taskExchangeWrite(void)
{
  while(true)
  {
    Delay(20);
    if(Panel->flags.enableEx == false) continue;

    writeDevice();
  }
}


void writeDeviceMaster(MemTypes_t port, uint8_t number)
{
  
}

void writeDeviceSlave(MemTypes_t port, uint8_t number)
{
  size_t i;
  for(i = 0; i < 2; i++)
  {
    if(PSW[1100] & (1 << (number + 8*i)))
    {
      cell_t c;
      c.type = port; c.adress = 1; c.number = 2 + i; c.value = PSW[1000 + 2*number + i];
      if(write(c).status == memStatusOK)
        PSW[1100] &= ~(1 << (number + 8*i));
    }
  }
}

void writeDevice(void)
{
  size_t i;

  switch (getMyIP())
  {
  case 41:
  case 42:

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

void readDevice(void)
{
  size_t i;
  struct FlagsPanel_s flags;
  cell_t c;
  uint16_t temp[256] = {0};

  c.adress = 1;

  switch (getMyIP())
  {
  case 41: 
  case 42: 

    c.type = net3; c.number = FIRST_RR_PANEL + (&CAST_TO_U16(Panel->flags) - &CAST_TO_U16(*Panel)); c.ptr = &CAST_TO_U16(flags);
    Panel->flags.errConMaster = reads(c, sizeof(flags)/2).status != memStatusOK ? true : false;
    
    c.type = net0; c.number = 2500; c.ptr = PSW + 2500;
    Panel->flags.errConPanel1 = reads(c, 4).status != memStatusOK ? true : false;
    if(Panel->flags.errConPanel1 == false)
    {
      c.number = 2508; c.ptr = PSW + 2508;
      Panel->flags.errConPanel1 = reads(c, 8).status != memStatusOK ? true : false;
    }

    c.type = net1; c.number = 2504; c.ptr = PSW + 2504;
    Panel->flags.errConPanel2 = reads(c, 4).status != memStatusOK ? true : false;
    if(Panel->flags.errConPanel2 == false)
    {
      c.number = 2516; c.ptr = PSW + 2516;
      Panel->flags.errConPanel2 = reads(c, 8).status != memStatusOK ? true : false;
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
    c.type = net3; c.number = 2500; c.ptr = temp;
    reads(c, 24);

    for(i = 0; i < 4; i++) 
      PSW[2504+i] = temp[4+i];
    for(i = 0; i < 8; i++) 
      PSW[2516+i] = temp[16+i];
    break;
    
  case 44:
    c.type = net3; c.number = 2500; c.ptr = temp;
    reads(c, 24);

    for(i = 0; i < 4; i++) 
      PSW[2500+i] = temp[0+i];
    for(i = 0; i < 8; i++) 
      PSW[2508+i] = temp[8+i];
    break;
  
  default:
    break;
  }
}
