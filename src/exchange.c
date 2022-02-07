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

#define CHECK_EQUALS_EVENT(_E1_, _E2_)                                                      \
  (((_E1_).Day != (_E2_).Day || (_E1_).Month != (_E2_).Month || (_E1_).Year != (_E2_).Year  \
  || (_E1_).Hour != (_E2_).Hour || (_E1_).Min != (_E2_).Min || (_E1_).Sec != (_E2_).Sec     \
  || (_E1_).Event != (_E2_).Event) ? false : true)

#define WRITE_STATE_TEST(_NET_)           \
  c.type = (_NET_); c = write(c);         \
  if(c.status == memStatusOK) {           \
    c2.type = (_NET_);                    \
    c2.number = &Panel->numberTest - PSW; \
    c2.value = Panel->numberTest;         \
    write(c2);                            \
  }

//-------------------------
void writeAlarms(void);
void controlTest(void);
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

    controlTest();

    readDevice();
  }
}

void taskExchangeReadGateWrite(void)
{
  while(true)
  {
    cell_t c, c_alarm;
    size_t i;

  tryAgain:
    if(Panel->flags.enableEx == false) { Delay(20); continue; }

    // if(Panel->flags.chooseTestMode == true)
    // {
    //   writeDevice(false);
    //   Delay(20); continue;
    // }

    Delay(300);
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
      if(PFW->N_Event != tempSettEvent[0].N_Event ) 
      {
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

      if(GetPSBStatus(702) == false)
      {
        GO_AGAIN_READS(net2, 120, &CAST_TO_U16(dMem->Gate), c, CALC_COUNT_RR(dGatemem_t)-1);
      }
      
      if(dMem->Gate->errCon.SHOT == true || GetPSBStatus(702) == true)
      {
        memset(&CAST_TO_U16(dMem->Gate->SHOT), 0, sizeof(dMem->Gate->SHOT));
        memset(&CAST_TO_U16(dMem->Gate->SHOT_ConfPanel), 0, sizeof(dMem->Gate->SHOT_ConfPanel));
        memset(&CAST_TO_U16(dMem->Gate->SettEvent[3]), 0, 
          &CAST_TO_U8(dMem->Gate->errCon) - &CAST_TO_U8(dMem->Gate->SettEvent[3]));
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
        
        CONTINUE_READS(net2, 299, &temp, c, 1);

        if((CAST_TO_U16(temp) & (1 << i)) == false 
          // && CAST_TO_U16(dMem->Gate->errCon) == temp
          && PFW->gateSettEvent[i].N_Event != dMem->Gate->SettEvent[i].N_Event
        ) {
          const uint16_t number = (
            dMem->Gate->SettEvent[i].CB ? COUNT_EVENTS : dMem->Gate->SettEvent[i].N_Event
          ) * NUMBER_RR_FOR_ONE_EVENT;
          const uint8_t max_k = number%125 == 0 ? number/125 : number/125 + 1;
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
      }
    }
  }
}

void taskExchangeWrite(void)
{
  while(true)
  {
    Delay(10);
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
  bool_t otherWrite = false;

  if(Panel->flags.can == false) return;

  for(i = 0; i < 2; i++)
  {
    otherWrite = false;
    otherWrite = (Panel->flags.chooseTestMode && (PSW[1100] & (1 << (number + 8*i))) && (
      (number == 0 && Panel->numberTest == 1 && ((i == 0 && (PSW[2502] & 0xFF0F) != (PSW[1000] & 0xFF0F))
        || (i == 1 && (PSW[2503] & 0x000F) != (PSW[1001] & 0x000F)))) ||
      (number == 0 && Panel->numberTest == 3 && ((i == 0 && (PSW[2502] & 0xFFF0) != (PSW[1000] & 0xFFF0))
        || (i == 1 && (PSW[2503] & 0x000F) != (PSW[1001] & 0x000F)))) ||
      (number == 1 && Panel->numberTest == 2 && ((i == 0 && (PSW[2506] & 0xFF0F) != (PSW[1002] & 0xFF0F))
        || (i == 1 && (PSW[2507] & 0x000F) != (PSW[1003] & 0x000F)))) ||
      (number == 1 && Panel->numberTest == 4 && ((i == 0 && (PSW[2506] & 0xFFF0) != (PSW[1002] & 0xFFF0))
        || (i == 1 && (PSW[2507] & 0x000F) != (PSW[1003] & 0x000F)))) ||
      (number == 2 && Panel->numberTest == 3) ||
      (number == 3 && Panel->numberTest == 1) ||
      (number == 4 && Panel->numberTest == 2) ||
      (number == 5 && Panel->numberTest == 4)
    ));

    if(!otherWrite)
    {
      PSW[1100] &= ~(1 << (number + 8*i));
    }
    else
    {
      cell_t c;
      c.type = port; c.adress = 1; c.number = 2 + i; c.value = PSW[1000 + 2*number + i];
      // PSW[2502 + 4*number + i] = c.value;
      // if(write(c).status == memStatusOK)
      write(c);
      PSW[1100] &= ~(1 << (number + 8*i));
    }
    if(PSW[1101] & (1 << (number + 8*i)))
    {
      cell_t c;
      c.type = port; c.adress = 1; c.number = 2 + i; c.value = PSW[1050 + 2*number + i];
      // PSW[2502 + 4*number + i] = c.value;
      // if(write(c).status == memStatusOK)
      write(c);
      PSW[1101] &= ~(1 << (number + 8*i));
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

void writeAlarms(void)
{
  size_t i, n;
  cell_t c;

  for(n = 0; n < 4; n++) for(i = 0; i < 2; i++)
  {
    CONTINUE_IF(GetPSBStatus(700+i)); 
    c.type = net0 + i; 
    c.number = &Alarms[alarmsActual + n]->count - PSW;

    CONTINUE_IF(GetPSBStatus(700+i)); 
    c = read(c);
    if(/* c.value != Alarms[alarmsActual + n]->count &&  */c.status == memStatusOK)
    {
      c.ptr = &CAST_TO_U16(*Alarms[alarmsActual + n]);
      c.number = c.ptr - PSW;
      writes(c, CALC_COUNT_RR(Alarms_t));
    }
  }
}

void writeEvents(void)
{
  size_t i, n, k;
  cell_t c;

  for(n = 0; n < 1; n++) for(i = 0; i < 2; i++)
  {
    CONTINUE_IF(GetPSBStatus(700+i)); 
    c.type = net0 + i; 
    c.number = &PFW->N_Event - PSW;

    c = read(c);
    CONTINUE_IF(c.value == PFW->N_Event || c.status != memStatusOK);

    for(k = 0; k < (PFW->CB ? COUNT_EVENTS : PFW->N_Event); k++)
    {
      EventByte_t temp[2];
      
      c.ptr = &CAST_TO_U16(temp[0]);
      c.type = memPFW; c.number = FIRST_RR_EVENT + k * NUMBER_RR_FOR_ONE_EVENT; 
      CONTINUE_IF(GetPSBStatus(700+i)); 
      reads(c, NUMBER_RR_FOR_ONE_EVENT);

      c.ptr = &CAST_TO_U16(temp[1]);
      c.type = net0 + i; c.number += 10000;
      CONTINUE_IF(GetPSBStatus(700+i)); 
      reads(c, NUMBER_RR_FOR_ONE_EVENT);

      if(!CHECK_EQUALS_EVENT(temp[0], temp[1]))
      {
        c.ptr = &CAST_TO_U16(temp[0]);
        c = writes(c, N_RR_FOR_EVENT);
        PSW[3000]++;
      }

      if(c.status != memStatusOK) break;
    }
    if(c.status == memStatusOK)
    {
      c.type = net0 + i; c.ptr = &PFW->N_Event; c.number = c.ptr - PSW;
      writes(c, 2);
    }
  }

  for(n = 0; n < 3; n++) for(i = 0; i < 2; i++)
  {
    CONTINUE_IF(GetPSBStatus(700+i)); 
    c.type = net0 + i; 
    c.ptr = &CAST_TO_U16(Panel->gateEventsState[n]);
    c.number = c.ptr - PSW;
    c = writes(c, CALC_COUNT_RR(Panel->gateEventsState[n]));

    c.number = &PFW->gateSettEvent[n].N_Event - PSW; 
    c = read(c);
    CONTINUE_IF(c.value == PFW->gateSettEvent[n].N_Event || c.status != memStatusOK);

    for(k = 0; k < (PFW->gateSettEvent[n].CB ? COUNT_EVENTS : PFW->gateSettEvent[n].N_Event); k++)
    {
      EventByte_t temp[2];
      
      c.ptr = &CAST_TO_U16(temp[0]);
      c.type = memPFW; 
      c.number = FIRST_RR_EVENT_GATE + (n * COUNT_EVENTS + k) * NUMBER_RR_FOR_ONE_EVENT; 
      CONTINUE_IF(GetPSBStatus(700+i)); 
      reads(c, NUMBER_RR_FOR_ONE_EVENT);

      c.ptr = &CAST_TO_U16(temp[1]);
      c.type = net0 + i; 
      c.number += 10000;
      CONTINUE_IF(GetPSBStatus(700+i)); 
      reads(c, NUMBER_RR_FOR_ONE_EVENT);

      if(!CHECK_EQUALS_EVENT(temp[0], temp[1]))
      {
        c.ptr = &CAST_TO_U16(temp[0]);
        c = writes(c, N_RR_FOR_EVENT);
        PSW[3002]++;
      }

      if(c.status != memStatusOK) break;
    }
    if(c.status == memStatusOK)
    {
      c.type = net0 + i; c.ptr = &PFW->gateSettEvent[n].N_Event; c.number = c.ptr - PSW;
      writes(c, 2);
    }
  }
}

void checkClearEvents(void)
{
  size_t i, n, k;
  cell_t c;

  // bool_t clearEvents = false;
  controlFlags_t flags;

  for(i = 0; i < 2; i++)
  {
    CONTINUE_IF(GetPSBStatus(700+i)); 
    c.type = net0 + i;
    c.number = &CAST_TO_U16(Panel->controlFlags) - PSW;
    CONTINUE_IF(GetPSBStatus(700+i)); 
    c = read(c);
    CONTINUE_IF(c.status != memStatusOK);

    CAST_TO_U16(flags) = c.value;
    if(flags.clearEvents)
    {
      Panel->controlFlags.clearEvents = true;
      flags.clearEvents = false;
      c.value = CAST_TO_U16(flags);
      write(c);

      PSW[3001]++;
    }
  }

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

void writeGateData(void)
{
  size_t i;
  cell_t c;

  for(i = 0; i < 2; i++)
  {
    c.type = net0 + i; 
    c.ptr = &CAST_TO_U16(dMem->Gate);
    c.number = c.ptr - PSW;
    writes(c, CALC_COUNT_RR(dMem->Gate));
  }
}

void syncTime(void)
{
  cell_t c;
  int i = -1;

  if(Panel->flags.isMaster && Panel->flags2.saveDataTime)
  {
    for(i = 0; i < 3; i++)
    {
      struct FlagsPanel2_s flags2;
      switch (i)
      {
      case 0: c.type = net0; break;
      case 1: c.type = net1; break;
      case 2: c.type = net3; break;
      }

      c.ptr = &CAST_TO_U16(flags2); c.number = &CAST_TO_U16(Panel->flags2) - PSW;
      reads(c, 1);
      
      if(flags2.saveDataTimeMaster == false)
      {
        flags2.saveDataTimeSlave = true;
        writes(c, 1);
        c.number = &CAST_TO_U16(Panel->newTime) - PSW; c.ptr = &CAST_TO_U16(dMem->time);
        writes(c, CALC_COUNT_RR(dMem->time));
      }
    }

    Panel->flags2.saveDataTime = false;

    return;
  }

  if(Panel->flags.isMaster)
  {
    struct FlagsPanel2_s flags2;
    c.number = &CAST_TO_U16(Panel->flags2) - PSW; 

    for(i = 0; i < 3; i++)
    {
      switch (i)
      {
      case 0: c.type = net0; break;
      case 1: c.type = net1; break;
      case 2: c.type = net3; break;
      }

      c.ptr = &CAST_TO_U16(flags2); 
      reads(c, 1);

      if(flags2.saveDataTimeMaster == true)
      {        
        c.number = &CAST_TO_U16(dMem->time) - PSW; c.ptr = &CAST_TO_U16(Panel->newTime);
        reads(c, CALC_COUNT_RR(dMem->time));

        c.number = &CAST_TO_U16(Panel->flags2) - PSW; c.ptr = &CAST_TO_U16(flags2); 
        flags2.saveDataTimeMaster = false;
        writes(c, 1);

        break;
      }
    }
  }
}

void updateCounters()
{
  cell_t c;
  if(getMyIP() == 41 || getMyIP() == 42)
  {
    c.number = &Panel->counterEx[getMyIP() == 41 ? 0 : 1] - PSW; 
    c.value = 0;

    c.type = net0; write(c);
    c.type = net1; write(c);
  }
}

void getValuesDP()
{
  cell_t c;

  if(getMyIP() == 43 || getMyIP() == 44) return;

  if(Panel->flags.isMaster)
  {
    c.type = net0; c.ptr = &CAST_TO_U16(dMem->DP[0]); c.number = c.ptr - PSW; 
    reads(c, CALC_COUNT_RR(dMem->DP[0]));
    c.type = net1; c.ptr = &CAST_TO_U16(dMem->DP[1]); c.number = c.ptr - PSW; 
    reads(c, CALC_COUNT_RR(dMem->DP[1]));
    c.type = net0; c.ptr = &CAST_TO_U16(dMem->DP[2]); c.number = c.ptr - PSW; 
    reads(c, CALC_COUNT_RR(dMem->DP[2])*2);
    c.type = net1; c.ptr = &CAST_TO_U16(dMem->DP[4]); c.number = c.ptr - PSW; 
    reads(c, CALC_COUNT_RR(dMem->DP[4])*2);
  }
  else
  {
    c.type = net3; c.ptr = &CAST_TO_U16(dMem->DP[0]); c.number = c.ptr - PSW; 
    reads(c, CALC_COUNT_RR(dMem->DP[0])*6);
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
  bool_t haveGate = false;

  c.adress = 1;

  switch (getMyIP())
  {
  case 41: 
  case 42: 
  {
    c.type = net3; 
    c.number = &CAST_TO_U16(Panel->flags) - PSW; 
    c.ptr = &CAST_TO_U16(flags);
    Panel->flags.errConMaster 
      = reads(c, sizeof(flags)/2).status != memStatusOK ? true : false;
    
    c.type = net0; c.number = 2500;
    Panel->flags.errConPanel1 = read(c).status != memStatusOK;
    c.type = net1; c.number = 2500;
    Panel->flags.errConPanel2 = read(c).status != memStatusOK;

    if(Panel->flags.errConMaster && Panel->flags.isMaster == false)
    {
      c.type = net2; c.number = 120; c.status = memStatusIGNORE;
      haveGate = read(c).status == memStatusOK;

      if(haveGate) Panel->flags.isMaster = true;
    }
    
    c.type = net3; 
    c.number = &CAST_TO_U16(Panel->flags) - PSW; 
    c.ptr = &CAST_TO_U16(flags);
    Panel->flags.errConMaster 
      = reads(c, sizeof(flags)/2).status != memStatusOK ? true : false;

    if(Panel->flags.errConMaster && haveGate) Panel->flags.isMaster = true;

    if(Panel->flags.errConMaster 
       && Panel->flags.errConPanel1 
       && Panel->flags.errConPanel2
       && haveGate == false)
    {
      Panel->flags.isMaster = false;
    }
    else if(Panel->flags.isMaster && flags.isMaster && Panel->flags.initMaster == false 
            && Panel->flags.errConMaster == false) 
    {
      Panel->flags.isMaster = false;
    }
    else if((Panel->flags.errConMaster == true && Panel->flags.errConMasterOld == true) 
            || (Panel->flags.errConMaster == false && flags.isMaster == false)) 
    {
      Panel->flags.isMaster = true;
    }

    Panel->flags.errConMasterOld = Panel->flags.errConMaster;

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

    updateCounters();

    getValuesDP();
        
    if(Panel->flags.isMaster)
    {
      writeAlarms();
      checkClearEvents();
      writeEvents();
      writeGateData();
    }

    if(Panel->flags.chooseTestMode == true)
    {
      writeDevice(false);
      // Delay(20); continue;
    }
    syncTime();

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
  }
  break;
      
  default:
    break;
  }
}

void controlTest(void)
{
  int i;
  cell_t c, c2;

  struct FlagsPanel_s flags;
  controlFlags_t cflags;
  switch (getMyIP())
  {
  case 41:
  case 42:
    if(Panel->flags.isMaster == false) break;

    for(i = 0; i < 2; i++)
    {
      c.type = net0 + i; c.number = &CAST_TO_U16(Panel->controlFlags) - PSW;
      c = read(c);
      CAST_TO_U16(cflags) = c.value;
      CONTINUE_IF(cflags.changeTest == false || c.status != memStatusOK);

      c.type = net0 + i; c.number = &CAST_TO_U16(Panel->flags) - PSW + 1;
      c = read(c);
      *(&CAST_TO_U16(flags) + 1) = c.value;

      c.number = &CAST_TO_U16(Panel->numberTest) - PSW;
      c = read(c);
      
      if(c.status == memStatusOK) SetPSB(750); else ResetPSB(750);
      if(flags.chooseTestMode)    SetPSB(751); else ResetPSB(751);
      if(cflags.changeTest)       SetPSB(752); else ResetPSB(752);

      if(c.status == memStatusOK 
        && (flags.chooseTestMode != Panel->flags.chooseTestMode || c.value != Panel->numberTest)
        && cflags.changeTest == true)
      {
        if(c.status == memStatusOK)
        {
          Panel->numberTest = c.value;
          Panel->controlFlags.invertTest = true;

          c.type = net0 + i; c.number = &CAST_TO_U16(Panel->controlFlags) - PSW;
          cflags.changeTest = false;
          c.value = CAST_TO_U16(cflags);
          write(c);
        }
      }
    }
    break;
  }

  if(Panel->controlFlags.invertTest == true)
  {
    if(getMyIP() == 43 || getMyIP() == 44) 
      Panel->controlFlags.changeTest = true;
    Panel->controlFlags.stateTest = (Panel->flags.chooseTestMode ? 2 : 1);
  }
  if(Panel->controlFlags.stateTest != 0)
  {
    controlFlags_t tt;
    tt = Panel->controlFlags;
    tt.invertTest = false;
    c.number = &CAST_TO_U16(Panel->controlFlags) - PSW; 
    c.value = CAST_TO_U16(tt);

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
      WRITE_STATE_TEST(net0);
      WRITE_STATE_TEST(net1);
      WRITE_STATE_TEST(net3);
      break;

    case 43:
    case 44:
      WRITE_STATE_TEST(net3);
      break;
    }
    Panel->controlFlags.invertTest = false;
  }
}

void connectionFaultHandler(uint8_t number)
{
  size_t i;

  if((getMyIP() == 43 || getMyIP() == 44) && number > 3)
  {
    if(Panel->counterEx[number-4] >= 150) SetPSB(700 + number); else ResetPSB(700 + number);
  }
  else
  {
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
  }
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
