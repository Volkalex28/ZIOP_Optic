/**
 * @file exchange.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include <string.h>

#include "alarms.h"
#include "lib.h"

#include "devices/devices.h"
#include "devices/devices_mem.h"

#include "mem/panel.h"

#define GO_AGAIN_IF(_STATE_) if(_STATE_) goto tryAgain
#define CONTINUE_IF(_STATE_) if(_STATE_) continue

#define GO_AGAIN_READS(_NET_, _NUM_, _PTR_, _CELL_, _COUNT_)                    \
  _CELL_##.type = _NET_; _CELL_##.number = _NUM_; _CELL_##.ptr = _PTR_;         \
  GO_AGAIN_IF(reads(_CELL_, _COUNT_).status != memStatusOK)

#define CONTINUE_READS(_NET_, _NUM_, _PTR_, _CELL_, _COUNT_)                    \
  _CELL_##.type = _NET_; _CELL_##.number = _NUM_; _CELL_##.ptr = _PTR_;         \
  if(reads(_CELL_, _COUNT_).status != memStatusOK) continue

#define GO_AGAIN_WRITE(_NET_, _NUM_, _CELL_, _VALUE_)                           \
  _CELL_##.type = _NET_; _CELL_##.number = _NUM_; _CELL_##.value = _VALUE_;     \
  GO_AGAIN_IF(write(_CELL_).status != memStatusOK)

#define CONTINUE_WRITE(_NET_, _NUM_, _CELL_, _VALUE_)                           \
  _CELL_##.type = _NET_; _CELL_##.number = _NUM_; _CELL_##.value = _VALUE_;     \
  if(write(_CELL_).status != memStatusOK) continue

//-------------------------
void writeDevice(void);
void readDevice(void);


void taskExchangeRead(void)
{
  while(true)
  {
    Delay(20);
    CONTINUE_IF(Panel->flags.enableEx == false);

    readDevice();
  }
}

void taskExchangeReadGate(void)
{
  while(true)
  {
    cell_t c, c_alarm;
    size_t i;

  tryAgain:

    Delay(300);
    GO_AGAIN_IF(Panel->flags.enableEx == false);

    if(Panel->flags.isMaster == false)
    {
      GO_AGAIN_IF(Panel->flags.errConMaster == true);

      GO_AGAIN_READS(net3, &CAST_TO_U16(dMem->Gate) - PSW, &CAST_TO_U16(dMem->Gate), 
        c, CALC_COUNT_RR(dGatemem_t)
      );

      GO_AGAIN_READS(net3, FIRST_RR_ALARMS_GATE, (uint16_t *)Alarms[alarmsSHOT], 
        c, CALC_COUNT_RR(Alarms_t)*3
      );
    }
    else
    {
      GO_AGAIN_READS(net2, 299, &CAST_TO_U16(dMem->Gate->errCon), c, 1);
      GO_AGAIN_READS(net2, 71, PSW + 3000, c, 3);

      if(dMem->Gate->errCon.SHOT == false)
      {
        GO_AGAIN_READS(net2, 120, &CAST_TO_U16(dMem->Gate), c, CALC_COUNT_RR(dGatemem_t)-1);
      }
      else memset(&CAST_TO_U16(dMem->Gate), 0, CALC_COUNT_RR(dGatemem_t)-1);

      for(i = 0; i < 3; i++)
      {
        size_t n;
        PSW[3005]++;

        if(CAST_TO_U16(dMem->Gate->errCon) & (1 << i))
        {
          memset(PSW + FIRST_RR_ALARMS_GATE + CALC_COUNT_RR(Alarms_t)*i, 0, sizeof(Alarms_t));
          continue;
        }

        CONTINUE_WRITE(net2, 49, c, 0x11 + i*2);

        c.value = 0;
        while(!(c.value & 0x8000) && n++ < 3 && !(CAST_TO_U16(dMem->Gate->errCon) & (1 << i))) 
        {
          c = read(c);
          if(c.status != memStatusOK) break;
          Delay(500);
        }
        CONTINUE_IF(CAST_TO_U16(dMem->Gate->errCon) & (1 << i));

        PSW[3006]++;

        CONTINUE_READS(net2, 300, PSW + FIRST_RR_ALARMS_GATE + CALC_COUNT_RR(Alarms_t)*i,
          c, CALC_COUNT_RR(Alarms_t)
        );
      }
    }
  }
}

void taskExchangeWrite(void)
{
  while(true)
  {
    Delay(20);
    CONTINUE_IF(Panel->flags.enableEx == false);

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

void readAlarms(void)
{
  size_t i, n;
  cell_t c;
  uint16_t temp[CALC_COUNT_RR(Alarms_t)] = {0};

  for(n = 0; n < 4; n++)
  {
    uint16_t number = (uint16_t *)Alarms[alarmsActual + n] - PSW;

    c.type = net4; c.number = number; c.ptr = temp;
    if(reads(c, CALC_COUNT_RR(Alarms_t)).status != memStatusOK)
    {
      c.type = net5;
      reads(c, CALC_COUNT_RR(Alarms_t));
    }
    for(i = 0; i < CALC_COUNT_RR(Alarms_t); i++)
    {
      PSW[number + i] = temp[i];
    }
  }
}

void readDevice(void)
{
  size_t i, n;
  struct FlagsPanel_s flags;
  cell_t c;
  uint16_t temp[25] = {0};

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
    
    if(Panel->flags.isMaster && flags.isMaster && Panel->flags.initMaster == false 
      && Panel->flags.errConMaster == false && Panel->flags.errConMasterOld == false
    ) Panel->flags.isMaster = false;

    if((Panel->flags.errConPanel1 == false && Panel->flags.errConPanel1Old == false) 
      || (Panel->flags.errConPanel2 == false && Panel->flags.errConPanel2Old == false)
    ) if((Panel->flags.errConMaster == true && Panel->flags.errConMasterOld == true) 
        || (Panel->flags.errConMaster == false && Panel->flags.errConMasterOld == false 
          && flags.initMaster == false && flags.isMaster == false
        )
      ) Panel->flags.isMaster = true;

    Panel->flags.errConMasterOld = Panel->flags.errConMaster;
    Panel->flags.errConPanel1Old = Panel->flags.errConPanel1;
    Panel->flags.errConPanel2Old = Panel->flags.errConPanel2;

    break;

  case 43:
    c.type = net3; c.number = 2500; c.ptr = temp;
    reads(c, 24);

    for(i = 0; i < 4; i++) 
      PSW[2504+i] = temp[4+i];
    for(i = 0; i < 8; i++) 
      PSW[2516+i] = temp[16+i];

    readAlarms();
    break;
    
  case 44:
    c.type = net3; c.number = 2500; c.ptr = temp;
    if(reads(c, 24).status == memStatusOK)
    {
      for(i = 0; i < 4; i++) 
        PSW[2500+i] = temp[0+i];
      for(i = 0; i < 8; i++) 
        PSW[2508+i] = temp[8+i];
    }

    readAlarms();
    break;
  
  default:
    break;
  }
}
