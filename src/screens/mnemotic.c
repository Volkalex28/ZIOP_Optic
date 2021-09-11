/**
 * @file shot.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"

// #include "..\alarms.h"
// #include "..\devices\dGate.h"
// #include "..\devices\devices_mem.h"
// #include "..\mem\panel.h"

uint16_t GroundingKnife (uint16_t reg, uint16_t offset);
uint16_t WithdrawableCircuitBreaker (uint16_t reg, uint16_t regime, uint16_t offset); 


void screenMnemotic(void)
{
  short i, k, temp;

  for (i = 0; i < 4; i++)
  {
    // Q1-7, Q1-8, Q1-9, Q1-10
    Screens->Mnemotic.Settings.RegimeScreen[i*2] = WithdrawableCircuitBreaker((2525+i*2), 0, 0); // 2525.0...2525.4
    Screens->Mnemotic.Settings.RegimeScreen[i*2+1] = GroundingKnife(2525+i*2,5);  //2525.5, 2525.6
    // QS-T1, QS-T2, QS-T3, QS-T4
    Screens->Mnemotic.Settings.RegimeScreen[8+i*2] = WithdrawableCircuitBreaker((2526+i*2), 2, 0); // 2526.0, 2526.1
    Screens->Mnemotic.Settings.RegimeScreen[9+i*2] = GroundingKnife(2526+i*2,2); // 2526.2, 2526.3
    // Q1, Q2, Q3, Q4
    Screens->Mnemotic.Settings.RegimeScreen[16+i*2] = WithdrawableCircuitBreaker((2526+i*2), 1, 4); //2526.4...2526.7
    Screens->Mnemotic.Settings.RegimeScreen[17+i*2] = GroundingKnife(2526+i*2,8); //2526.8, 2526.9
    // Q1-3, Q1-4, Q1-5, Q1-6
    Screens->Mnemotic.Settings.RegimeScreen[24+i] = WithdrawableCircuitBreaker((2526+i*2), 1, 10); //2526.10...2526.11
  }

  if (Screens->Mnemotic.Settings.Flags.EventOpenWindow)
  {
    OpenWindow(20,100,122);

    if (Screens->Mnemotic.Settings.TitleWindowNumber > 11)
      {
        Screens->Mnemotic.Settings.Flags.FlagVisible = true;
        Screens->Mnemotic.Settings.RegimeWindow1 = 
        Screens->Mnemotic.Settings.RegimeScreen[Screens->Mnemotic.Settings.TitleWindowNumber+12];
      }
    else
      {
        Screens->Mnemotic.Settings.Flags.FlagVisible = false;
        Screens->Mnemotic.Settings.RegimeWindow1 = 
        Screens->Mnemotic.Settings.RegimeScreen[Screens->Mnemotic.Settings.TitleWindowNumber*2];
        Screens->Mnemotic.Settings.RegimeWindow2 = 
        Screens->Mnemotic.Settings.RegimeScreen[Screens->Mnemotic.Settings.TitleWindowNumber*2+1];
      }
    // светики  
    switch (Screens->Mnemotic.Settings.TitleWindowNumber)
    {
    case 0: //Q1-9 T1
      Screens->Mnemotic.Settings.Flags.Led1 = (PSW[2534] & 1) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led2 = (PSW[2534] & (1<<1)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led3 = (PSW[2534] & (1<<2)) ? true : false;
      break;
    case 1: //Q1-8 T2
      Screens->Mnemotic.Settings.Flags.Led1 = (PSW[2535] & 1) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led2 = (PSW[2535] & (1<<1)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led3 = true;
      break;
    case 2: //Q1-7 T3
      Screens->Mnemotic.Settings.Flags.Led1 = (PSW[2534] & (1<<4)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led2 = (PSW[2534] & (1<<5)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led3 = true;
      break;
    case 3: //Q1-10 T4
      Screens->Mnemotic.Settings.Flags.Led1 = (PSW[2535] & (1<<4)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led2 = (PSW[2535] & (1<<5)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led3 = true;
      break;
    case 4: //QS-T1 T1
      Screens->Mnemotic.Settings.Flags.Led1 = (PSW[2536] & 1) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led2 = (PSW[2536] & (1<<1)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led3 = true;
      break;
    case 5: //QS-T2 T2
      Screens->Mnemotic.Settings.Flags.Led1 = (PSW[2537] & 1) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led2 = (PSW[2537] & (1<<1)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led3 = true;
      break;
    case 6: //QS-T3 T3
      Screens->Mnemotic.Settings.Flags.Led1 = (PSW[2538] & 1) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led2 = (PSW[2538] & (1<<1)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led3 = true;
      break;
    case 7: //QS-T4 T4
      Screens->Mnemotic.Settings.Flags.Led1 = (PSW[2539] & 1) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led2 = (PSW[2539] & (1<<1)) ? true : false;
      Screens->Mnemotic.Settings.Flags.Led3 = true;
      break;
    default:
      Screens->Mnemotic.Settings.Flags.Led1 = false;
      Screens->Mnemotic.Settings.Flags.Led2 = false;
      Screens->Mnemotic.Settings.Flags.Led3 = false;
      break;
    }
  }
  else
  {
    CloseWindow(20);
  }
}

uint16_t GroundingKnife (uint16_t reg, uint16_t offset)
{
  if (PSW[reg] & (1<<(offset+1)))
  {
    return Yellow; // yellow
  }
  else 
  {
    if (PSW[reg] & (1<<offset))
    {
      return Red; // red
    }
    else
    {
      return Green; // green
    }
  }
}

uint16_t WithdrawableCircuitBreaker (uint16_t reg, uint16_t regime, uint16_t offset) 
{
  short k;

  switch (regime)
  {
    case 2:
      switch ((PSW[reg] >> offset) & 0x3)  
      {
        case 0: return 10;
        case 1: return 9;
        case 2: return 11;
        case 3: return 12;
      }
    default:
      for (k = 0; k < 9; k++)
      {
        uint16_t temp = (PSW[reg] >> offset) & MaskaTrans[regime][k];
        if (temp == ValueTrans[regime][k])
        {
          return k;
        }
      }
    break;
  }
}
