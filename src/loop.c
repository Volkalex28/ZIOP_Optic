/**
 * @file loop.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

/**
 * @defgroup Core Основные модули
 * 
 * Содержит такие модули как "Основной цикл работы", "Обмен", 
 * "Аварийные и системные сообщения" и т.д.
 */

#include "lib.h"

#include "alarms.h"
#include "init.h"

#include "mem/panel.h"
#include "mem/pfw.h"

#include "screens/screen.h"

#define getBit(_RR_, _BIT_) ((PSW[_RR_] & (1 << (_BIT_))) ? true : false)
#define alExContr(_NC_, _COND_) if (_COND_) addCrash(_NC_); else deleteCrash(_NC_) 

void diffConrtol(uint16_t inNum, uint16_t outNum, uint8_t fbit, uint8_t sbit, uint8_t ofbit, uint8_t osbit)
{
  if((PSW[inNum + fbit/16] & (1 << (fbit%16))) && !(PSW[inNum + sbit/16] & (1 << (sbit%16))))
  {
    PSW[outNum + ofbit/16] &= ~(1 << (ofbit%16));
    PSW[outNum + osbit/16] &= ~(1 << (osbit%16));
  }
  else if(!(PSW[inNum + fbit/16] & (1 << (fbit%16))) && (PSW[inNum + sbit/16] & (1 << (sbit%16))))
  {
    PSW[outNum + ofbit/16] |= (1 << (ofbit%16));
    PSW[outNum + osbit/16] &= ~(1 << (osbit%16));
  }
  else
  {
    PSW[outNum + ofbit/16] &= ~(1 << (ofbit%16));
    PSW[outNum + osbit/16] |= (1 << (osbit%16));
  }
}

void normConrtol(uint16_t inNum, uint16_t outNum, uint8_t bit, uint8_t obit, bool_t inversion)
{
  if((PSW[inNum + bit/16] & (1 << (bit%16))))
    PSW[outNum + obit/16] |= (1 << (obit%16));
  else
    PSW[outNum + obit/16] &= ~(1 << (obit%16));
  
  if(inversion)
    PSW[outNum + obit/16] ^= (1 << (obit%16));
}

void handlerLogic(void)
{
  //- 51
  normConrtol(2500, 2533, 9, 0, false);
  normConrtol(2501, 2533, 3, 1, false);

  diffConrtol(2500, 2525, 0, 1, 0, 1);
  diffConrtol(2500, 2525, 2, 3, 3, 4);
  diffConrtol(2500, 2525, 4, 5, 5, 6);
  
  diffConrtol(2500, 2529, 10, 11, 0, 1);
  diffConrtol(2500, 2529, 12, 13, 3, 4);
  diffConrtol(2500, 2529, 14, 15, 5, 6);
  
  normConrtol(2502, 2534, 0, 0, true); // 2534.0 = Блокировка ВЭ выключателя Q1-9 = K1.DO1
  normConrtol(2502, 2534, 1, 1, true); // 2534.1 = Блокировка ЗЭ Q1-9 = K1.DO2
  normConrtol(2502, 2534, 2, 2, false); // 2534.2 = Отключение выключателя Q1-9 = K1.DO3

  normConrtol(2502, 2534, 4, 4, true); // 2534.4 = Блокировка ВЭ выключателя Q1-7 = K1.DO5
  normConrtol(2502, 2534, 5, 5, true); // 2534.5 = Блокировка ЗЭ Q1-7 = K1.DO6
  normConrtol(2502, 2534, 6, 6, false); // 2534.6 = Отключение выключателя Q1-7 = K1.DO7

  //- 52
  normConrtol(2504, 2533, 9, 2, false);
  normConrtol(2505, 2533, 3, 3, false);

  diffConrtol(2504, 2527, 0, 1, 0, 1);
  diffConrtol(2504, 2527, 2, 3, 3, 4);
  diffConrtol(2504, 2527, 4, 5, 5, 6);
  
  diffConrtol(2504, 2531, 10, 11, 0, 1);
  diffConrtol(2504, 2531, 12, 13, 3, 4);
  diffConrtol(2504, 2531, 14, 15, 5, 6);

  normConrtol(2506, 2535, 0, 0, true); // 2534.0 = Блокировка ВЭ выключателя Q1-8 = K2.DO1
  normConrtol(2506, 2535, 1, 1, true); // 2534.1 = Блокировка ЗЭ Q1-8 = K2.DO2
  normConrtol(2506, 2535, 2, 2, false); // 2534.2 = Отключение выключателя Q1-8 = K2.DO3

  normConrtol(2506, 2535, 4, 4, true); // 2535.4 = Блокировка ВЭ выключателя Q1-10 = K2.DO5
  normConrtol(2506, 2535, 5, 5, true); // 2535.5 = Блокировка ЗЭ Q1-10 = K2.DO6
  normConrtol(2506, 2535, 6, 6, false); // 2535.6 = Отключение выключателя Q1-10 = K2.DO7
  //- 53
  diffConrtol(2508, 2530, 0, 1, 0, 1);
  diffConrtol(2508, 2530, 2, 3, 2, 3);
  
  normConrtol(2508, 2529, 4, 7, false);
  normConrtol(2508, 2529, 5, 8, false);
  normConrtol(2508, 2529, 6, 9, false);
  normConrtol(2508, 2529, 7, 10, false);

  diffConrtol(2508, 2530, 8, 9, 4, 5);
  diffConrtol(2508, 2530, 10, 11, 6, 7);
  diffConrtol(2508, 2530, 12, 13, 8, 9);
  
  diffConrtol(2508, 2530, 15, 16, 10, 11);
  diffConrtol(2509, 2530, 1, 2, 12, 13);

  normConrtol(2510, 2536, 0, 0, true); // 2536.0 = Блокировка ВЭ разъединителя QS-T1 = K-T1.DO1
  normConrtol(2510, 2536, 1, 1, true); // 2536.1 = Блокировка ЗЭ разъединителя QSG-T1 = K-T1.DO2
  //- 54
  diffConrtol(2512, 2526, 0, 1, 0, 1);
  diffConrtol(2512, 2526, 2, 3, 2, 3);
  
  normConrtol(2512, 2525, 4, 7, false);
  normConrtol(2512, 2525, 5, 8, false);
  normConrtol(2512, 2525, 6, 9, false);
  normConrtol(2512, 2525, 7, 10, false);

  diffConrtol(2512, 2526, 8, 9, 4, 5);
  diffConrtol(2512, 2526, 10, 11, 6, 7);
  diffConrtol(2512, 2526, 12, 13, 8, 9);
  
  diffConrtol(2512, 2526, 15, 16, 10, 11);
  diffConrtol(2513, 2526, 1, 2, 12, 13);
  
  normConrtol(2513, 2533, 3, 4, false);

  normConrtol(2514, 2537, 0, 0, true); // 2537.0 = Блокировка ВЭ разъединителя QS-T3 = K-T3.DO1
  normConrtol(2514, 2537, 1, 1, true); // 2537.1 = Блокировка ЗЭ разъединителя QSG-T3 = K-T3.DO2
  //- 55
  diffConrtol(2516, 2528, 0, 1, 0, 1);
  diffConrtol(2516, 2528, 2, 3, 2, 3);
  
  normConrtol(2516, 2527, 4, 7, false);
  normConrtol(2516, 2527, 5, 8, false);
  normConrtol(2516, 2527, 6, 9, false);
  normConrtol(2516, 2527, 7, 10, false);

  diffConrtol(2516, 2528, 8, 9, 4, 5);
  diffConrtol(2516, 2528, 10, 11, 6, 7);
  diffConrtol(2516, 2528, 12, 13, 8, 9);
  
  diffConrtol(2516, 2528, 15, 16, 10, 11);
  diffConrtol(2517, 2528, 1, 2, 12, 13);
  
  normConrtol(2517, 2533, 3, 5, false);

  normConrtol(2518, 2538, 0, 0, true); // 2538.0 = Блокировка ВЭ разъединителя QS-T2 = K-T2.DO1
  normConrtol(2518, 2538, 1, 1, true); // 2538.1 = Блокировка ЗЭ разъединителя QSG-T2 = K-T2.DO2
  //- 56
  diffConrtol(2520, 2532, 0, 1, 0, 1);
  diffConrtol(2520, 2532, 2, 3, 2, 3);
  
  normConrtol(2520, 2531, 4, 7, false);
  normConrtol(2520, 2531, 5, 8, false);
  normConrtol(2520, 2531, 6, 9, false);
  normConrtol(2520, 2531, 7, 10, false);

  diffConrtol(2520, 2532, 8, 9, 4, 5);
  diffConrtol(2520, 2532, 10, 11, 6, 7);
  diffConrtol(2520, 2532, 12, 13, 8, 9);
  
  diffConrtol(2520, 2532, 15, 16, 10, 11);
  diffConrtol(2521, 2532, 1, 2, 12, 13);

  normConrtol(2522, 2539, 0, 0, true); // 2539.0 = Блокировка ВЭ разъединителя QS-T4 = K-T4.DO1
  normConrtol(2522, 2539, 1, 1, true); // 2539.1 = Блокировка ЗЭ разъединителя QSG-T4 = K-T4.DO2
}

void controlLogic(void)
{
  // if(PSW[2502] != PSW[2508] && getMyIP() == 43 && (PSW[1100] & (1<<0)) == 0)
  // {
  //   PSW[1000] = PSW[2508];
  //   PSW[1100] |= (1<<0);
  // }
  // if(PSW[2506] != PSW[2508] && getMyIP() == 44 && (PSW[1100] & (1<<1)) == 0)
  // {
  //   PSW[1002] = PSW[2508];
  //   PSW[1100] |= (1<<1);
  // }
  if(getBit(2525, 0) == false && getBit(2525, 1) == false && getBit(2525, 3) == true && getBit(2525, 4) == false
    && getBit(2526, 0) == true && getBit(2526, 1) == false && getBit(2526, 4) == true && getBit(2526, 5) == false
    && getBit(2526, 6) == false && getBit(2526, 7) == false && getBit(2526, 10) == true && getBit(2526, 11) == false
    && getBit(2526, 12) == false && getBit(2526, 13) == false
  ) {
    if(getBit(2502, 1) == false)
    {
      PSW[1000] |= 0x2;
      PSW[1100] |= (1<<0);
    }
    if(getBit(2510, 1) == false)
    {
      PSW[1004] |= 0x2;
      PSW[1100] |= (1<<2);
    }
  }
  else
  {
    if(getBit(2502, 0) == true)
    {
      PSW[1000] &= ~0x2;
      PSW[1100] |= (1<<0);
    }
    if(getBit(2510, 0) == true)
    {
      PSW[1004] &= ~0x2;
      PSW[1100] |= (1<<2);
    }
  }

  if(getBit(2525, 0) == false && getBit(2525, 1) == false && getBit(2525, 5) == false && getBit(2525, 6) == false
    && getBit(2526, 2) == false && getBit(2526, 3) == false && getBit(2526, 4) == true && getBit(2526, 5) == false
    && getBit(2526, 8) == false && getBit(2526, 9) == false && getBit(2526, 10) == true && getBit(2526, 11) == false
  ) {
    if(getBit(2502, 0) == false)
    {
      PSW[1000] |= 0x1;
      PSW[1100] |= (1<<0);
    }
    if(getBit(2510, 0) == false)
    {
      PSW[1004] |= 0x1;
      PSW[1100] |= (1<<2);
    }
  }
  else
  {
    if(getBit(2502, 0) == true)
    {
      PSW[1000] &= ~0x1;
      PSW[1100] |= (1<<0);
    }
    if(getBit(2510, 0) == true)
    {
      PSW[1004] &= ~0x1;
      PSW[1100] |= (1<<2);
    }
  }
}

void alarmLogic(void)
{
  short i;

  switch (getMyIP())
  {
    case 41:  // Panel 43, Panel 44, Gate, Panel 42
    
      alExContr(alConFailPanel3, (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailPanel4, (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailGate,   (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel2, (getEnable(3) == false) || GetPSBStatus(703));
      break;
    case 42:  // Panel 43, Panel 44, Gate, Panel 41
      alExContr(alConFailPanel3, (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailPanel4, (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailGate,   (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel1, (getEnable(3) == false) || GetPSBStatus(703));
      break;
    case 43:  // K-1, KT-1, KT-3, Panel 44, Panel 42, Panel 41
      alExContr(alConFailDP1,    (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailDP3,    (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailDP4,    (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel4, (getEnable(3) == false) || GetPSBStatus(703));
      alExContr(alConFailPanel2, (getEnable(4) == false) || GetPSBStatus(704));
      alExContr(alConFailPanel1, (getEnable(5) == false) || GetPSBStatus(705));
      break;
    case 44:  // K-2, KT-2, KT-4, Panel 43, Panel 41, Panel 42
      alExContr(alConFailDP2,    (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailDP5,    (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailDP6,    (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel4, (getEnable(3) == false) || GetPSBStatus(703));
      alExContr(alConFailPanel1, (getEnable(4) == false) || GetPSBStatus(704));
      alExContr(alConFailPanel2, (getEnable(5) == false) || GetPSBStatus(705));
      break;
  }

}

void taskLoop(void)
{
  init();

  addEvent(alPowerOn1 + getMyIP() - 41);
  Panel->flags.enableEx = true;

  while(true)
  {
    int i = 0;
    cell_t c;

    clearRRScreens();
    switch (PSW[CURRENT_SCREEN])
    {
      case scrMnemotic: screenMnemotic(); break;
      case scrConfAlarms:
        screenConfCrash();
        selectDeviceInCrashAndEvent();
        break;
      case scrZVU:   screenZvu();   break;
      case scrBKI:   screenBkif();  break;
      case scrSHOT:  screenShot();  break;
      case scrSHSN:  screenShsn();  break;
      case scrCrash:
        screenCrash();
        selectDeviceInCrashAndEvent();
        break;
      case scrEvent:
        screenEvent();
        selectDeviceInCrashAndEvent();
        break;

      default:
        break;
    }
    fillRRScreens();

    if((getMyIP() == 41 || getMyIP() == 42) && Panel->flags.isMaster == true) 
      fillCrash();

    updateScreen();
    controlMenu();

    for(i = 0; i < 6; i++) connectionFaultHandler(i);

    handlerLogic();
    controlLogic();
    alarmLogic();

    updatePFW();
    getTime();
    Delay(50);
  }
}
