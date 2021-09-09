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
    _CELL_ = read(_CELL_);                                                            \
    if(_CELL_##.status != memStatusOK) break;                                         \
    Delay(500);                                                                       \
  }                                                                                   \
  CONTINUE_IF(CHECK_FOR_WAIT(_NUM_DEV_, _COUNTER_, _COUNT_VAL_) == false)

#define N_RR_FOR_EVENT 125

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
  } break;

  case 43:
  case 44:
    c.type = net3; c.number = &CAST_TO_U16(*dMem->DP) - PSW; c.ptr = &CAST_TO_U16(*tempDP);
    reads(c, numberRR_DP*N_DP);

    for(n = 0; n < 2; n++) for(i = 0; i < 4 * (n + 1); i++) 
      dMem->DP[getMyIP() == 43 ? 1 + 3*n : 2*n].DIO.regs[i] 
        = tempDP[getMyIP() == 43 ? 1 +3*n : 2*n].DIO.regs[i];

    readAlarms();

    if(PSW[CURRENT_SCREEN] != scrEvent)
      break;
tryAgain:
    for(n = 0; n < 2; n++)
    {
      CONTINUE_IF(GetPSBStatus(704 + n) == true);

      CONTINUE_READS(net4 + n, &CAST_TO_U16(dMem->Gate) - PSW, &CAST_TO_U16(dMem->Gate), 
        c, CALC_COUNT_RR(dGatemem_t)
      );

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
    break;
      
  default:
    break;
  }
}


void connectionFaultHandler(uint8_t number)
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
