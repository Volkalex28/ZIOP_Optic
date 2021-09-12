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
#include "mem/pfw.h"

#include "screens/screen.h"

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

#define CHECK_FOR_WAIT(_NUM_DEV_, _COUNTER_, _COUNT_VAL_)                       \
  (((_COUNTER_) < (_COUNT_VAL_))                                                \
  && (CAST_TO_U16(dMem->Gate->errCon) & (1 << (_NUM_DEV_))) == false)

#define CONTINUE_WAIT(_NET_, _NUM_, _CELL_, _TAG_, _NUM_DEV_, _COUNTER_, _COUNT_VAL_) \
  CONTINUE_WRITE(_NET_, _NUM_, _CELL_, _TAG_ + 2*(_NUM_DEV_));                        \
  _CELL_##.value = 0; _COUNTER_ = 0;                                                  \
  while((_CELL_##.value & 0x8000) == false                                            \
    && CHECK_FOR_WAIT(_NUM_DEV_, _COUNTER_, _COUNT_VAL_)                              \
  ) {                                                                                 \
    _COUNTER_##++;                                                                    \
    GO_AGAIN_IF(Panel->flags.chooseTestMode == true);                                 \
    _CELL_ = read(_CELL_);                                                            \
    if(_CELL_##.status != memStatusOK) break;                                         \
    Delay(500);                                                                       \
  }                                                                                   \
  CONTINUE_IF(CHECK_FOR_WAIT(_NUM_DEV_, _COUNTER_, _COUNT_VAL_) == false)

#define ABS(_VAL) (_VAL > 0 ? _VAL : -1 * _VAL)

#define CALC_MINUTES(_TIME_)                             \
  ( _TIME_##.Day*1440 + _TIME_##.Hour*60 + _TIME_##.Min  \
  + _TIME_##.Month*44640)

#define CHECK_EQUALS_TIME(_TIME1_, _TIME2_)              \
  ((ABS(CALC_MINUTES(_TIME1_) - CALC_MINUTES(_TIME2_)) <= 1) ? true : false) 

#define CHECK_ZERO_TIME(_TIME_) \
  (( _TIME_##.Day    != 0       \
  || _TIME_##.Hour   != 0       \
  || _TIME_##.Min    != 0       \
  || _TIME_##.Month  != 0       \
  || _TIME_##.Sec    != 0       \
  || _TIME_##.Year   != 0) ? false : true)

#define N_RR_FOR_EVENT 125

//-------------------------
void writeDevice(bool_t enableSlave);
void readDevice(void);
void connectionAlarmsHandler(MemTypes_t port, Alarm_t number, bool_t isDelete);
void connectionEventsHandler(MemTypes_t port, Alarm_t number);

void taskExchangeRead(void)
{
  cell_t c;

  while(true)
  {
    Delay(20);
    CONTINUE_IF(Panel->flags.enableEx == false);

    if(Panel->controlFlags.invertTest == true)
    {
      Panel->controlFlags.stateTest = (Panel->flags.chooseTestMode ? 2 : 1);
    }
    if(Panel->controlFlags.stateTest != 0)
    {
      controlFlags_t t;
      t = Panel->controlFlags;
      t.invertTest = false;
      c.number = &CAST_TO_U16(Panel->controlFlags) - PSW; c.value = CAST_TO_U16(t);

      switch (Panel->controlFlags.stateTest)
      {
      case 1: Panel->flags.chooseTestMode = true;   break;
      case 2: Panel->flags.chooseTestMode = false;  break;
      
      default:
        break;
      }

      Panel->controlFlags.stateTest = 0;
    }
    if(Panel->controlFlags.invertTest == true)
    {
      switch (getMyIP())
      {
      case 41:
      case 42:
        c.type = net0; write(c);
        c.type = net1; write(c);
        c.type = net3; write(c);
        break;

      case 43:
      case 44:
        c.type = net4; write(c);
        c.type = net5; write(c);
        c.type = net3; write(c);
        break;
      }
      Panel->controlFlags.invertTest = false;
    }

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

    if(Panel->flags.chooseTestMode == true)
    {
      writeDevice(false);
      continue;
    }

    if(Panel->flags.isMaster == false)
    {
      size_t n, k;
      uint16_t temp;
      GateSettEvent_t tempSettEvent[3];

      GO_AGAIN_IF(Panel->flags.errConMaster == true);

      GO_AGAIN_READS(net3, &CAST_TO_U16(dMem->Gate) - PSW, &CAST_TO_U16(dMem->Gate), 
        c, CALC_COUNT_RR(dGatemem_t)
      );

      GO_AGAIN_READS(net3, FIRST_RR_ALARMS, (uint16_t *)Alarms[alarmsActual], 
        c, CALC_COUNT_RR(Alarms_t)
      );

      GO_AGAIN_READS(net3, FIRST_RR_ALARMS_GATE, (uint16_t *)Alarms[alarmsSHOT], 
        c, CALC_COUNT_RR(Alarms_t)*3
      );

      GO_AGAIN_READS(net3, &CAST_TO_U16(PFW->N_Event) - PSW, &CAST_TO_U16(tempSettEvent), 
        c, CALC_COUNT_RR(GateSettEvent_t)
      );
      if(PFW->N_Event != tempSettEvent[0].N_Event
      ) {
        const uint16_t number = (tempSettEvent[0].CB ? COUNT_EVENTS 
          : tempSettEvent[0].N_Event) * NUMBER_RR_FOR_ONE_EVENT;
        const uint8_t max_k = number%N_RR_FOR_EVENT == 0 ? number/N_RR_FOR_EVENT : number/N_RR_FOR_EVENT + 1;

        for(k = 0; k < max_k; k++)
        {
          uint16_t temp[N_RR_FOR_EVENT] = {0};

          GO_AGAIN_IF(GetPSBStatus(703) == true);
          
          CONTINUE_READS(net3, 
            FIRST_RR_EVENT + 10000 + N_RR_FOR_EVENT*k, 
            temp, 
            c, 
            N_RR_FOR_EVENT
          );
          
          c.type = memPFW; c.number = FIRST_RR_EVENT + N_RR_FOR_EVENT*k;
          writes(c, N_RR_FOR_EVENT);
        }
        PFW->CB = tempSettEvent[0].CB;
        PFW->N_Event = tempSettEvent[0].N_Event;
      }

      GO_AGAIN_READS(net3, &CAST_TO_U16(PFW->gateSettEvent) - PSW, &CAST_TO_U16(tempSettEvent), 
        c, CALC_COUNT_RR(dMem->Gate->SettEvent)
      );
      for(i = 0; i < 3; i++)
      {
        if(CAST_TO_U16(dMem->Gate->errCon) & (1 << i))
        {
          Panel->gateEventsState[i].isError = true;
          Panel->gateEventsState[i].percent = 0;
          Panel->gateEventsState[i].isWait = false;
        }
        else
        {
          Panel->gateEventsState[i].isError = false;
        }

        if(tempSettEvent[i].N_Event != dMem->Gate->SettEvent[i].N_Event)
        {
          Panel->gateEventsState[i].isWait = true;
          continue;
        }
        else
        {
          Panel->gateEventsState[i].isWait = false;
        }

        if((CAST_TO_U16(dMem->Gate->errCon) & (1 << i)) == false 
          && PFW->gateSettEvent[i].N_Event != dMem->Gate->SettEvent[i].N_Event
        ) {
          const uint16_t number = (dMem->Gate->SettEvent[i].CB ? COUNT_EVENTS 
            : dMem->Gate->SettEvent[i].N_Event) * NUMBER_RR_FOR_ONE_EVENT;
          const uint8_t max_k = number%N_RR_FOR_EVENT == 0 ? number/N_RR_FOR_EVENT : number/N_RR_FOR_EVENT + 1;

          for(k = 0; k < max_k; k++)
          {
            uint16_t temp[N_RR_FOR_EVENT] = {0};

            GO_AGAIN_IF(GetPSBStatus(703) == true);

            Panel->gateEventsState[i].percent = (float)k/max_k*100;
            
            CONTINUE_READS(net3, 
              FIRST_RR_EVENT_GATE + 10000 + COUNT_EVENTS*NUMBER_RR_FOR_ONE_EVENT*i + N_RR_FOR_EVENT*k, 
              temp, 
              c, 
              N_RR_FOR_EVENT
            );
            
            c.type = memPFW; c.number = FIRST_RR_EVENT_GATE + COUNT_EVENTS*NUMBER_RR_FOR_ONE_EVENT*i + N_RR_FOR_EVENT*k;
            writes(c, N_RR_FOR_EVENT);
          }
          Panel->gateEventsState[i].percent = 100;
          PFW->gateSettEvent[i].CB = dMem->Gate->SettEvent[i].CB;
          PFW->gateSettEvent[i].N_Event = dMem->Gate->SettEvent[i].N_Event;
        }

        if((dMem->Gate->SettEvent[i].CB != 0 || dMem->Gate->SettEvent[i].N_Event != 0)
          && PFW->gateSettEvent[i].N_Event == dMem->Gate->SettEvent[i].N_Event
          && PFW->gateSettEvent[i].CB == dMem->Gate->SettEvent[i].CB
        ) {
          Panel->gateEventsState[i].percent = 100;
          Panel->gateEventsState[i].isWait = false;
          Panel->gateEventsState[i].isError = false;
        }
      }
    }
    else
    {
      for(i = 0; i < 3; i++)
      {
        c.type = net2; c.number = 49;
        if(CAST_TO_U16(dMem->Gate->errCon) & (1 << i) || read(c).status != memStatusOK)
        {
          Panel->gateEventsState[i].isError = true;
          Panel->gateEventsState[i].percent = 0;
          Panel->gateEventsState[i].isWait = false;
        }
        else
        {
          Panel->gateEventsState[i].isError = false;
        }
      }
      GO_AGAIN_READS(net2, 299, &CAST_TO_U16(dMem->Gate->errCon), c, 1);
      GO_AGAIN_READS(net2, 71, PSW + 3000, c, 3);

      if(dMem->Gate->errCon.SHOT == false || GetPSBStatus(702) == true)
      {
        GO_AGAIN_READS(net2, 120, &CAST_TO_U16(dMem->Gate), c, CALC_COUNT_RR(dGatemem_t)-1);
      }
      
      if(dMem->Gate->errCon.SHOT == true || GetPSBStatus(702) == true)
      {
        memset(&CAST_TO_U16(dMem->Gate->SHOT), 0, sizeof(dMem->Gate->SHOT));
        memset(&CAST_TO_U16(dMem->Gate->SHSN[2]), 0, 
          sizeof(dMem->Gate) - sizeof(dMem->Gate->SHOT) - sizeof(dMem->Gate->SHSN) - 2);
      }
      if(dMem->Gate->errCon.SHSN == true || GetPSBStatus(702) == true)
        memset(&CAST_TO_U16(dMem->Gate->SHSN[0]), 0, sizeof(dMem->Gate->SHSN[0]));
      if(dMem->Gate->errCon.SHSND == true)
        memset(&CAST_TO_U16(dMem->Gate->SHSN[1]), 0, sizeof(dMem->Gate->SHSN[1]));

      for(i = 0; i < 3; i++)
      {
        size_t n, k;
        Alarms_t alTemp;
        uint16_t temp;

        c.type = net2; c.number = 49;
        if(CAST_TO_U16(dMem->Gate->errCon) & (1 << i) || read(c).status != memStatusOK)
        {
          memset(PSW + FIRST_RR_ALARMS_GATE + CALC_COUNT_RR(Alarms_t)*i, 0, sizeof(Alarms_t));
          Panel->gateEventsState[i].isError = true;
          Panel->gateEventsState[i].percent = 0;
          Panel->gateEventsState[i].isWait = false;
          continue;
        }
        else
        {
          Panel->gateEventsState[i].isError = false;
        }

        CONTINUE_WAIT(net2, 49, c, 0x11, i, n, 10);

        CONTINUE_READS(net2, 300, &CAST_TO_U16(alTemp), c, CALC_COUNT_RR(Alarms_t));

        Alarms[alarmsSHOT+i]->count = alTemp.count;
        for(k = 0; k < COUNT_ALARMS; k++)
        {
          Alarms[alarmsSHOT+i]->buf[k] = alTemp.buf[k];
          alTemp.buf[k] = 0;
        }
        alTemp.count = 0;
        
        // c.type = net2; c.number = 49; c.ptr = ;
        // c = reads(c, 1);
        CONTINUE_READS(net2, 299, &temp, c, 1);

        if((CAST_TO_U16(dMem->Gate->errCon) & (1 << i)) == false 
          && CAST_TO_U16(dMem->Gate->errCon) == temp
          && PFW->gateSettEvent[i].N_Event != dMem->Gate->SettEvent[i].N_Event
        ) {
          const uint16_t number = (dMem->Gate->SettEvent[i].CB 
            ? COUNT_EVENTS 
            : dMem->Gate->SettEvent[i].N_Event
          ) * NUMBER_RR_FOR_ONE_EVENT;
          const uint8_t max_k = number%125 == 0 
            ? number/125 
            : number/125 + 1;
          uint16_t temp[125] = {0};

          Panel->gateEventsState[i].isWait = true;
          CONTINUE_WAIT(net2, 49, c, 0x10, i, n, 30);
          Panel->gateEventsState[i].isWait = false;

          for(k = 0; k < max_k; k++)
          {
            GO_AGAIN_IF(GetPSBStatus(702) == true);
            GO_AGAIN_IF(Panel->flags.chooseTestMode == true);

            Panel->gateEventsState[i].percent = (float)k/max_k*100;
            
            c.type = net2; c.number = 299;
            CONTINUE_IF(read(c).value & (1 << i));
            CONTINUE_READS(net2, 300 + 125*k, temp, c, 125);
            
            c.type = memPFW; c.number = FIRST_RR_EVENT_GATE + COUNT_EVENTS*NUMBER_RR_FOR_ONE_EVENT*i + 125*k;
            writes(c, 125);
          }
          Panel->gateEventsState[i].percent = 100;
          PFW->gateSettEvent[i].CB = dMem->Gate->SettEvent[i].CB;
          PFW->gateSettEvent[i].N_Event = dMem->Gate->SettEvent[i].N_Event;
        }

        if((dMem->Gate->SettEvent[i].CB != 0 || dMem->Gate->SettEvent[i].N_Event != 0)
          && PFW->gateSettEvent[i].N_Event == dMem->Gate->SettEvent[i].N_Event
          && PFW->gateSettEvent[i].CB == dMem->Gate->SettEvent[i].CB
        ) {
          Panel->gateEventsState[i].percent = 100;
          Panel->gateEventsState[i].isWait = false;
          Panel->gateEventsState[i].isError = false;
        }
        
        // c.type = net2; c.number = 49; c.ptr = &CAST_TO_U16(dMem->Gate->errCon);
        // c = reads(c, 1);
        // if(c.status == memStatusOK && (CAST_TO_U16(dMem->Gate->errCon) & (1 << i)) == false)
        // {
        // }
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

    writeDevice(true);
  }
}


void writeDeviceMaster(MemTypes_t port, uint8_t number)
{
  size_t i;
  if(Panel->flags.chooseTestMode == false) return;
  if((PSW[1100] & (1 << number)) || (PSW[1100] & (1 << (number + 8))))
  {
    cell_t c;
    c.type = port; c.adress = 1; c.number = 1000 + 2*number; c.ptr = PSW + c.number;
    if(writes(c, 2).status == memStatusOK)
    {
      c.number = 1100; c.value = PSW[c.number];
      if(write(c).status == memStatusOK)
      {
        PSW[1100] &= ~(1 << number);
        PSW[1100] &= ~(1 << (number + 8));
      }
    }
  }
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
      PSW[2502 + 4*number + i] = c.value;
      if(write(c).status == memStatusOK)
        PSW[1100] &= ~(1 << (number + 8*i));
    }
  }
}

void writeDevice(bool_t enableSlave)
{
  size_t i;

  switch (getMyIP())
  {
  case 41:
  case 42:
    writeDeviceMaster(net0, 0);
    writeDeviceMaster(net1, 1);
    writeDeviceMaster(net0, 2);
    writeDeviceMaster(net0, 3);
    writeDeviceMaster(net1, 4);
    writeDeviceMaster(net1, 5);
    break;

  case 43: 
    if(enableSlave == false) return;
    writeDeviceSlave(net0, 0); 
    writeDeviceSlave(net1, 2); 
    writeDeviceSlave(net2, 3); 
    writeDeviceMaster(net3, 1);
    writeDeviceMaster(net3, 4);
    writeDeviceMaster(net3, 5);
    break;
    
  case 44: 
    if(enableSlave == false) return;
    writeDeviceSlave(net0, 1); 
    writeDeviceSlave(net1, 4); 
    writeDeviceSlave(net2, 5); 
    writeDeviceMaster(net3, 0);
    writeDeviceMaster(net3, 2);
    writeDeviceMaster(net3, 3);
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
    uint16_t number = &CAST_TO_U16(*Alarms[alarmsActual + n]) - PSW;

    c.type = net4; c.number = number; c.ptr = temp;
    if(reads(c, CALC_COUNT_RR(Alarms_t)).status != memStatusOK)
    {
      c.type = net5;
      reads(c, CALC_COUNT_RR(Alarms_t));
    }
    // if(PSW[CURRENT_SCREEN] != scrCrash) break;

    for(i = 0; i < CALC_COUNT_RR(Alarms_t); i++)
    {
      PSW[number + i] = temp[i];
    }
  }
}

void readDevice(void)
{
  size_t i, n, k;
  struct FlagsPanel_s flags;
  cell_t c;
  dDPmem_t tempDP[N_DP];
  uint16_t temp;
  GateSettEvent_t tempSettEvent[3];
  const uint16_t numberRR_DP = CALC_COUNT_RR(dMem->DP->DIO);

  c.adress = 1;

  switch (getMyIP())
  {
  case 41: 
  case 42: 
  {
    c.type = net3; 
    c.number = FIRST_RR_PANEL + (&CAST_TO_U16(Panel->flags) - &CAST_TO_U16(*Panel)); 
    c.ptr = &CAST_TO_U16(flags);
    Panel->flags.errConMaster 
      = reads(c, sizeof(flags)/2).status != memStatusOK ? true : false;

    for(n = 0; n < 2; n++) for(i = 0; i < 2; i++)
      if((CAST_TO_U16(Panel->flags) & (1 << (n+7))) == false || i == 0)
      {
        c.type = (n == 0 ? net0 : net1); 
        c.ptr = &CAST_TO_U16(dMem->DP[(i+1)*n + 2*i]); 
        c.number = c.ptr - PSW;
        
        if(reads(c, numberRR_DP*(1+i)).status != memStatusOK || getEnable(n) == false) 
          CAST_TO_U16(Panel->flags) |= (1 << (n+7));
        else 
          CAST_TO_U16(Panel->flags) &= ~(1 << (n+7));
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

    connectionAlarmsHandler(net0, alConFailDP1, true);
    connectionAlarmsHandler(net1, alConFailDP2, true);
    connectionAlarmsHandler(net0, alConFailDP3, true);
    connectionAlarmsHandler(net0, alConFailDP4, true);
    connectionAlarmsHandler(net1, alConFailDP5, true);
    connectionAlarmsHandler(net1, alConFailDP6, true);

    connectionEventsHandler(net3, alPowerOn1);
    connectionEventsHandler(net3, alPowerOn2);
    connectionEventsHandler(net0, alPowerOn3);
    connectionEventsHandler(net1, alPowerOn4);

    connectionEventsHandler(net3, alOpenUserAccess1);
    connectionEventsHandler(net3, alOpenUserAccess2);
    connectionEventsHandler(net0, alOpenUserAccess3);
    connectionEventsHandler(net1, alOpenUserAccess4);

    connectionEventsHandler(net3, alOpenAdminAccess1);
    connectionEventsHandler(net3, alOpenAdminAccess2);
    connectionEventsHandler(net0, alOpenAdminAccess3);
    connectionEventsHandler(net1, alOpenAdminAccess4);

    if(Panel->controlFlags.clearEvents == true)
    {
      if(Panel->flags.isMaster == true)
        clearEvents(false);
      else if(Panel->flags.errConMaster == false)
      {
        controlFlags_t f;
        cell_t c; c.type = net3; c.number = &CAST_TO_U16(Panel->controlFlags) - PSW;

        f.clearEvents = true;
        c.value = CAST_TO_U16(Panel->controlFlags);
        write(c);
      }

      Panel->controlFlags.clearEvents = false;
    }
  } 
  break;

  case 43:
  case 44:
  {
    c.type = net3; c.number = &CAST_TO_U16(*dMem->DP) - PSW; c.ptr = &CAST_TO_U16(*tempDP);
    reads(c, numberRR_DP*N_DP);

    for(n = 0; n < 2; n++) for(i = 0; i < 4 * (n + 1); i++) 
      dMem->DP[getMyIP() == 43 ? 1 + 3*n : 2*n].DIO.regs[i] 
        = tempDP[getMyIP() == 43 ? 1 +3*n : 2*n].DIO.regs[i];
    
    readAlarms();

    if(Panel->controlFlags.clearEvents == true)
    {
      size_t i;
      controlFlags_t f;
      f.clearEvents = true;

      for(i = 0; i < 2; i++)
      {
        cell_t c; c.type = net4 + i; c.number = &CAST_TO_U16(Panel->controlFlags) - PSW;

        c.value = CAST_TO_U16(f);
        c = write(c);

        if(c.status == memStatusOK) break;
      }

      Panel->controlFlags.clearEvents = false;
    }
tryAgain:
    for(n = 0; n < 2; n++)
    {
      CONTINUE_IF(GetPSBStatus(704 + n) == true);

      CONTINUE_READS(net4 + n, &CAST_TO_U16(dMem->Gate) - PSW, &CAST_TO_U16(dMem->Gate), 
        c, CALC_COUNT_RR(dGatemem_t)
      );

      if(PSW[CURRENT_SCREEN] != scrEvent) break;

      CONTINUE_READS(net4 + n, &CAST_TO_U16(PFW->N_Event) - PSW, &CAST_TO_U16(tempSettEvent), 
        c, CALC_COUNT_RR(GateSettEvent_t)
      );
      if(PFW->N_Event != tempSettEvent[0].N_Event
      ) {
        const uint16_t number = (tempSettEvent[0].CB ? COUNT_EVENTS 
          : tempSettEvent[0].N_Event) * NUMBER_RR_FOR_ONE_EVENT;
        const uint8_t max_k = number%N_RR_FOR_EVENT == 0 ? number/N_RR_FOR_EVENT : number/N_RR_FOR_EVENT + 1;

        for(k = 0; k < max_k; k++)
        {
          uint16_t temp[N_RR_FOR_EVENT] = {0};

          GO_AGAIN_IF(GetPSBStatus(704 + n) == true);
          
          CONTINUE_READS(net4 + n, 
            FIRST_RR_EVENT + 10000 + N_RR_FOR_EVENT*k, 
            temp, 
            c, 
            N_RR_FOR_EVENT
          );
          
          c.type = memPFW; c.number = FIRST_RR_EVENT + N_RR_FOR_EVENT*k;
          writes(c, N_RR_FOR_EVENT);
        }
        PFW->CB = tempSettEvent[0].CB;
        PFW->N_Event = tempSettEvent[0].N_Event;
      }

      CONTINUE_READS(net4 + n, &CAST_TO_U16(PFW->gateSettEvent) - PSW, &CAST_TO_U16(tempSettEvent), 
        c, CALC_COUNT_RR(dMem->Gate->SettEvent)
      );
      for(i = 0; i < 3; i++)
      {
        if(CAST_TO_U16(dMem->Gate->errCon) & (1 << i))
        {
          Panel->gateEventsState[i].isError = true;
          Panel->gateEventsState[i].percent = 0;
          Panel->gateEventsState[i].isWait = false;
        }
        else
        {
          Panel->gateEventsState[i].isError = false;
        }

        if(tempSettEvent[i].N_Event != dMem->Gate->SettEvent[i].N_Event)
        {
          Panel->gateEventsState[i].isWait = true;
          continue;
        }
        else
        {
          Panel->gateEventsState[i].isWait = false;
        }

        if((CAST_TO_U16(dMem->Gate->errCon) & (1 << i)) == false 
          && PFW->gateSettEvent[i].N_Event != dMem->Gate->SettEvent[i].N_Event
        ) {
          const uint16_t number = (dMem->Gate->SettEvent[i].CB ? COUNT_EVENTS 
            : dMem->Gate->SettEvent[i].N_Event) * NUMBER_RR_FOR_ONE_EVENT;
          const uint8_t max_k = number%N_RR_FOR_EVENT == 0 ? number/N_RR_FOR_EVENT : number/N_RR_FOR_EVENT + 1;

          for(k = 0; k < max_k; k++)
          {
            uint16_t temp[N_RR_FOR_EVENT] = {0};

            GO_AGAIN_IF(GetPSBStatus(704 + n) == true);

            Panel->gateEventsState[i].percent = (float)k/max_k*100;
            
            CONTINUE_READS(net4 + n, 
              FIRST_RR_EVENT_GATE + 10000 + COUNT_EVENTS*NUMBER_RR_FOR_ONE_EVENT*i + N_RR_FOR_EVENT*k, 
              temp, 
              c, 
              N_RR_FOR_EVENT
            );
            
            c.type = memPFW; c.number = FIRST_RR_EVENT_GATE + COUNT_EVENTS*NUMBER_RR_FOR_ONE_EVENT*i + N_RR_FOR_EVENT*k;
            writes(c, N_RR_FOR_EVENT);
          }
          Panel->gateEventsState[i].percent = 100;
          PFW->gateSettEvent[i].CB = dMem->Gate->SettEvent[i].CB;
          PFW->gateSettEvent[i].N_Event = dMem->Gate->SettEvent[i].N_Event;
        }

        if((dMem->Gate->SettEvent[i].CB != 0 || dMem->Gate->SettEvent[i].N_Event != 0)
          && PFW->gateSettEvent[i].N_Event == dMem->Gate->SettEvent[i].N_Event
          && PFW->gateSettEvent[i].CB == dMem->Gate->SettEvent[i].CB
        ) {
          Panel->gateEventsState[i].percent = 100;
          Panel->gateEventsState[i].isWait = false;
          Panel->gateEventsState[i].isError = false;
        }
      }
      break;
    }
  }
  break;
      
  default:
    break;
  }
}


void connectionFaultHandler(uint8_t number)
{
  size_t i;

  if(Panel->StateEx[number].CounterCorrect > Panel->StateExOld[number].CounterCorrect
    && Panel->StateEx[number].CounterNotCorrect == Panel->StateExOld[number].CounterNotCorrect
  ) {
    Panel->StateExOld[number].CounterNotCorrect = Panel->StateEx[number].CounterNotCorrect;
    ResetPSB(700 + number);
  }
  else if(Panel->StateEx[number].CounterNotCorrect > Panel->StateExOld[number].CounterNotCorrect
    && Panel->StateEx[number].CounterCorrect == Panel->StateExOld[number].CounterCorrect
  ) {
    Panel->StateExOld[number].CounterCorrect = Panel->StateEx[number].CounterCorrect;
    SetPSB(700 + number);

    if((getMyIP() == 43 || getMyIP() == 44) && getEnable(number) == true && number < 3)
      setEnable(number, false);
  }

  Panel->StateExOld[number].CounterCorrect = Panel->StateEx[number].CounterCorrect;
  Panel->StateExOld[number].CounterNotCorrect = Panel->StateEx[number].CounterNotCorrect;

  PSW[2960 + number] = Panel->StateExOld[number].CounterCorrect;
  PSW[2970 + number] = Panel->StateExOld[number].CounterNotCorrect;

  PSW[2980 + number] = Panel->StateEx[number].CounterCorrect;
  PSW[2990 + number] = Panel->StateEx[number].CounterNotCorrect;

  // if(getMyIP() == 43)
  // {
  //   Panel->flags.errConC1 = GetPSBStatus(700);
  //   Panel->flags.errConC3 = GetPSBStatus(701);
  //   Panel->flags.errConC4 = GetPSBStatus(702);
  // }
  // else if(getMyIP() == 44)
  // {
  //   Panel->flags.errConC2 = GetPSBStatus(700);
  //   Panel->flags.errConC5 = GetPSBStatus(701);
  //   Panel->flags.errConC6 = GetPSBStatus(702);
  // }
}

void connectionAlarmsHandler(MemTypes_t port, Alarm_t number, bool_t isDelete)
{
  Alarms_t Alarms_temp;

  if(port == net0 || port == net1)
  {
    cell_t c;
    bool_t stateCon = (port == net0 ? Panel->flags.errConPanel1 : Panel->flags.errConPanel2);

    if(stateCon == false)
    {
      c.type = port; c.number = &CAST_TO_U16(*Alarms[alarmsBacklog]) - PSW; c.ptr = &CAST_TO_U16(Alarms_temp);
      c = reads(c, CALC_COUNT_RR(Alarms_t));
    }
    if(stateCon == false && c.status == memStatusOK && findAlarms(&Alarms_temp, number)) 
      addCrash(number);
    else if(isDelete) deleteCrash(number);
  }
}

void connectionEventsHandler(MemTypes_t port, Alarm_t number)
{
  Time_t Times_temp[4];
  Time_t otherTime;
  cell_t c;
  bool_t stateCon;
  Alarm_t firstAl = alNone;
  uint8_t num = 0;

  if(alPowerOn1 <= number && number <= alPowerOn4) { firstAl = alPowerOn1; num = 0; }
  else if(alOpenUserAccess1 <= number && number <= alOpenUserAccess4) { firstAl = alOpenUserAccess1; num = 1; }
  else if(alOpenAdminAccess1 <= number && number <= alOpenAdminAccess4) { firstAl = alOpenAdminAccess1; num = 2; }
  else return;

  if(port == net0 || port == net1)
    stateCon = (port == net0 ? Panel->flags.errConPanel1 : Panel->flags.errConPanel2);
  else if(port == net3 && (number - firstAl) != (getMyIP() - 41))
    stateCon = Panel->flags.errConMaster;
  else return;

  if(stateCon == false)
  {
    c.type = port; c.number = &CAST_TO_U16(PFW->statePanelsEvents[num][0]) - PSW; c.ptr = &CAST_TO_U16(Times_temp);
    c = reads(c, CALC_COUNT_RR(Times_temp));
  }

  if(c.status == memStatusOK && stateCon == false)
  {
    c.number = &CAST_TO_U16(*getTime()) - PSW; c.ptr = &CAST_TO_U16(otherTime);
    c = reads(c, CALC_COUNT_RR(otherTime));
    Times_temp[number - firstAl] = Times_temp[number - firstAl];
    
    if(c.status == memStatusOK 
      && CHECK_EQUALS_TIME(Times_temp[number - firstAl], PFW->statePanelsEvents[num][number - firstAl]) == false
      && CHECK_ZERO_TIME(Times_temp[number - firstAl]) == false
    ) {

      if(c.status == memStatusOK)
        addEventTime(number, Times_temp[number - firstAl]);
    }
  }
}
