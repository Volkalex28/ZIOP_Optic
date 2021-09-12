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

#include "devices/dGate.h"

#include "screens/screen.h"

#define getBit(_RR_, _BIT_) ((PSW[_RR_] & (1 << (_BIT_))) ? true : false)
#define alExContr(_NC_, _COND_) if (_COND_) addCrash(_NC_); else deleteCrash(_NC_) 
#define controlBit(_N_, _BIT_, _EN_, _DIS_, _COND_)                   \
  if(_COND_) {                                                        \
    if(getBit(2502 + ((_BIT_) / 16) + 4*(_N_), _BIT_) == false && (_EN_)) {            \
      PSW[1000 + ((_BIT_) / 16) + 2*(_N_)] |= (1 << (_BIT_));                          \
      PSW[1100] |= (1 << (_N_ + 8*((_BIT_) / 16)));                   \
    }                                                                 \
  } else {                                                            \
    if(getBit(2502 + 4*(_N_), _BIT_) == true && (_DIS_)) {            \
      PSW[1000 + 2*(_N_)] &= ~(1 << (_BIT_));                         \
      PSW[1100] |= (1 << (_N_ + 8*((_BIT_) / 16)));                   \
    }                                                                 \
  }

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
  normConrtol(2502, 2534, 2, 2, true); // 2534.2 = Отключение выключателя Q1-9 = K1.DO3

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
  normConrtol(2510, 2536, 2, 2, true); 
  normConrtol(2510, 2536, 3, 3, true); 
  //- 54
  diffConrtol(2512, 2526, 0, 1, 0, 1);
  diffConrtol(2512, 2526, 2, 3, 2, 3);
  
  normConrtol(2512, 2525, 4, 7, false);
  normConrtol(2512, 2525, 5, 8, false);
  normConrtol(2512, 2525, 6, 9, false);
  normConrtol(2512, 2525, 7, 10, false);
  normConrtol(2512, 2525, 14, 11, false);

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
  controlBit(0, 0, true, true, getBit(2525, 0) == false && getBit(2525, 1) == false && getBit(2525, 5) == false 
    && getBit(2525, 6) == false && getBit(2526, 2) == false && getBit(2526, 3) == false && getBit(2526, 4) == true 
    && getBit(2526, 5) == false && getBit(2526, 8) == false && getBit(2526, 9) == false && getBit(2526, 10) == true 
    && getBit(2526, 11) == false)
  controlBit(2, 0, true, true, getBit(2525, 0) == false && getBit(2525, 1) == false && getBit(2525, 5) == false 
    && getBit(2525, 6) == false && getBit(2526, 2) == false && getBit(2526, 3) == false && getBit(2526, 4) == true 
    && getBit(2526, 5) == false && getBit(2526, 8) == false && getBit(2526, 9) == false && getBit(2526, 10) == true 
    && getBit(2526, 11) == false)

  controlBit(0, 1, true, true, getBit(2525, 0) == false && getBit(2525, 1) == false && getBit(2525, 3) == true 
    && getBit(2525, 4) == false && getBit(2526, 0) == true && getBit(2526, 1) == false && getBit(2526, 4) == true 
    && getBit(2526, 5) == false && getBit(2526, 6) == false && getBit(2526, 7) == false && getBit(2526, 10) == true 
    && getBit(2526, 11) == false && getBit(2526, 12) == false && getBit(2526, 13) == false)
  controlBit(2, 1, true, true, getBit(2525, 0) == false && getBit(2525, 1) == false && getBit(2525, 3) == true 
    && getBit(2525, 4) == false && getBit(2526, 0) == true && getBit(2526, 1) == false && getBit(2526, 4) == true 
    && getBit(2526, 5) == false && getBit(2526, 6) == false && getBit(2526, 7) == false && getBit(2526, 10) == true 
    && getBit(2526, 11) == false && getBit(2526, 12) == false && getBit(2526, 13) == false)
    
  controlBit(0, 2, true, true, getBit(2525, 5) == false && getBit(2525, 6) == false && getBit(2526, 8) == false 
    && getBit(2526, 9) == false && getBit(2526, 2) == false && getBit(2526, 3) == false && getBit(2526, 0) == false 
    && getBit(2526, 1) == false && !(getBit(2525, 7) == true || getBit(2525, 11) == true) && getBit(2525, 10) == false
    && getBit(2525, 8) == false)

  controlBit(2, 2, true, true, (getBit(2525, 0) == true && getBit(2525, 1) == false && getBit(2526, 0) == false 
    && getBit(2526, 1) == false) && !(getBit(2525, 0) == false && getBit(2525, 1) == false) && getBit(2525, 9) == false)

  controlBit(2, 3, true, true, (getBit(2526, 4) == false && getBit(2526, 5) == false) && !(getBit(2525, 0) == false 
    && getBit(2525, 1) == false) && getBit(2525, 9) == false && !(getBit(2526, 4) == true && getBit(2526, 5) == false))
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
      alExContr(alConFailAtAllPanel,
        ((getEnable(0) == false) || GetPSBStatus(700)) &&
        ((getEnable(1) == false) || GetPSBStatus(701)) &&
        ((getEnable(2) == false) || GetPSBStatus(702)) &&
        ((getEnable(3) == false) || GetPSBStatus(703)) 
      );
      alExContr(alConFailShot, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHOT && Panel->flags.isMaster)); 
      alExContr(alConFailShsn, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHSN && Panel->flags.isMaster)); 
      alExContr(alConFailShsnD, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHSND && Panel->flags.isMaster)); 

      break;
    case 42:  // Panel 43, Panel 44, Gate, Panel 41
      alExContr(alConFailPanel3, (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailPanel4, (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailGate,   (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel1, (getEnable(3) == false) || GetPSBStatus(703));
      alExContr(alConFailAtAllPanel,
        ((getEnable(0) == false) || GetPSBStatus(700)) &&
        ((getEnable(1) == false) || GetPSBStatus(701)) &&
        ((getEnable(2) == false) || GetPSBStatus(702)) &&
        ((getEnable(3) == false) || GetPSBStatus(703)) 
      );
      alExContr(alConFailShot, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHOT && Panel->flags.isMaster)); 
      alExContr(alConFailShsn, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHSN && Panel->flags.isMaster)); 
      alExContr(alConFailShsnD, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHSND && Panel->flags.isMaster)); 
      break;
    case 43:  // K-1, KT-1, KT-3, Panel 44, Panel 42, Panel 41
      alExContr(alConFailDP1,    (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailDP3,    (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailDP4,    (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel4, (getEnable(3) == false) || GetPSBStatus(703));
      alExContr(alConFailPanel2, (getEnable(4) == false) || GetPSBStatus(704));
      alExContr(alConFailPanel1, (getEnable(5) == false) || GetPSBStatus(705));
      alExContr(alConFailAtAllPanel,
        ((getEnable(0) == false) || GetPSBStatus(700)) &&
        ((getEnable(1) == false) || GetPSBStatus(701)) &&
        ((getEnable(2) == false) || GetPSBStatus(702)) &&
        ((getEnable(3) == false) || GetPSBStatus(703)) &&
        ((getEnable(4) == false) || GetPSBStatus(704)) &&
        ((getEnable(5) == false) || GetPSBStatus(705))
      );
      break;
    case 44:  // K-2, KT-2, KT-4, Panel 43, Panel 41, Panel 42
      alExContr(alConFailDP2,    (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailDP5,    (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailDP6,    (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel4, (getEnable(3) == false) || GetPSBStatus(703));
      alExContr(alConFailPanel1, (getEnable(4) == false) || GetPSBStatus(704));
      alExContr(alConFailPanel2, (getEnable(5) == false) || GetPSBStatus(705));
      alExContr(alConFailAtAllPanel,
        ((getEnable(0) == false) || GetPSBStatus(700)) &&
        ((getEnable(1) == false) || GetPSBStatus(701)) &&
        ((getEnable(2) == false) || GetPSBStatus(702)) &&
        ((getEnable(3) == false) || GetPSBStatus(703)) &&
        ((getEnable(4) == false) || GetPSBStatus(704)) &&
        ((getEnable(5) == false) || GetPSBStatus(705))
      );
      break;
  }

}
 
void taskLoop(void)
{
  init();

  Panel->flags.isMaster = true;
  addEvent(alPowerOn1 + getMyIP() - 41);
  Panel->flags.isMaster = false;
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

    fillCrash();

    updateScreen();
    controlMenu();

    for(i = 0; i < 6; i++) connectionFaultHandler(i);

    handlerLogic();
    controlLogic();
    alarmLogic();

    updatePFW();
    getTime();

    if(GetAdminLevelAvtorisation != Panel->flags.StateAdminAccessOld)
    {
      Panel->flags.StateAdminAccessOld = !Panel->flags.StateAdminAccessOld;
      if(GetAdminLevelAvtorisation) 
      {
        addEvent(alOpenAdminAccess1 + getMyIP() - 41);
        SetUserLevelAvtorisation;
      }
    }
    if(GetUserLevelAvtorisation != Panel->flags.StateUserAccessOld)
    {
      Panel->flags.StateUserAccessOld = !Panel->flags.StateUserAccessOld;
      if(GetUserLevelAvtorisation) addEvent(alOpenUserAccess1 + getMyIP() - 41);
    }

    Delay(20);
  }
}
