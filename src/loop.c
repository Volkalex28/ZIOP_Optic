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

void normConrtol(uint16_t inNum, uint16_t outNum, uint8_t bit, uint8_t obit);
void diffConrtol(uint16_t inNum, uint16_t outNum, uint8_t fbit, uint8_t sbit, uint8_t ofbit, uint8_t osbit);
void controlLogic(void);
void handlerLogic(void);

void taskLoop(void)
{
  init();

  addEvent(alPowerOn1 + getMyIP() - 41);

  while(true)
  {

    clearRRScreens();
    switch (PSW[CURRENT_SCREEN])
    {
    case  5:
      screenConfCrash();
      selectDeviceInCrashAndEvent();
      break;
    case  6: screenZvu();   break;
    case  7: screenBkif();  break;
    case  8: screenShot();  break;
    case  9: screenShsn();  break;
    case 10:
      screenCrash();
      selectDeviceInCrashAndEvent();
      break;
    case 11:
      screenEvent();
      selectDeviceInCrashAndEvent();
      break;

    default:
      break;
    }
    fillRRScreens();

    if(Panel->oldScreen != PSW[CURRENT_SCREEN]) 
    {
      Panel->flags.menuIsOpen = false;
      if (PSW[CURRENT_SCREEN] == 5)
        Panel->ChooseDevice.ResetCrashList = 1;
    }
    if(Panel->flags.menuIsOpen) 
      OpenWindow(10, 600, 68); 
    else
      CloseWindow(10);
  
    // readMaskMessages();
    // fillCrash();

    handlerLogic();
    // controlLogic();

    Panel->oldScreen = PSW[CURRENT_SCREEN];
    updatePFW();
    time();
    Delay(50);
  }
}

void controlLogic(void)
{
  if(PSW[2502] != PSW[2508] && getMyIP() == 43 && (PSW[900] & (1<<0)) == 0)
  {
    PSW[1000] = PSW[2508];
    PSW[900] |= (1<<0);
  }
  if(PSW[2506] != PSW[2508] && getMyIP() == 44 && (PSW[900] & (1<<1)) == 0)
  {
    PSW[1002] = PSW[2508];
    PSW[900] |= (1<<1);
  }
}

void handlerLogic(void)
{
  //- 51
  normConrtol(2500, 2533, 9, 0);
  normConrtol(2501, 2533, 3, 1);

  diffConrtol(2500, 2525, 1, 0, 0, 1);
  diffConrtol(2500, 2525, 2, 3, 3, 4);
  diffConrtol(2500, 2525, 4, 5, 5, 6);
  
  diffConrtol(2500, 2529, 11, 10, 0, 1);
  diffConrtol(2500, 2529, 12, 13, 3, 4);
  diffConrtol(2500, 2529, 14, 15, 5, 6);
  //- 52
  normConrtol(2504, 2533, 9, 2);
  normConrtol(2505, 2533, 3, 3);

  diffConrtol(2504, 2527, 1, 0, 0, 1);
  diffConrtol(2504, 2527, 2, 3, 3, 4);
  diffConrtol(2504, 2527, 4, 5, 5, 6);
  
  diffConrtol(2504, 2531, 11, 10, 0, 1);
  diffConrtol(2504, 2531, 12, 13, 3, 4);
  diffConrtol(2504, 2531, 14, 15, 5, 6);
  //- 53
  diffConrtol(2508, 2530, 0, 1, 0, 1);
  diffConrtol(2508, 2530, 2, 3, 2, 3);
  
  normConrtol(2508, 2529, 4, 7);
  normConrtol(2508, 2529, 5, 8);
  normConrtol(2508, 2529, 6, 9);
  normConrtol(2508, 2529, 7, 10);

  diffConrtol(2508, 2530, 8, 9, 4, 5);
  diffConrtol(2508, 2530, 10, 11, 6, 7);
  diffConrtol(2508, 2530, 12, 13, 8, 9);
  
  diffConrtol(2508, 2530, 15, 16, 10, 11);
  diffConrtol(2509, 2530, 1, 2, 12, 13);
  //- 54
  diffConrtol(2512, 2526, 0, 1, 0, 1);
  diffConrtol(2512, 2526, 2, 3, 2, 3);
  
  normConrtol(2512, 2525, 4, 7);
  normConrtol(2512, 2525, 5, 8);
  normConrtol(2512, 2525, 6, 9);
  normConrtol(2512, 2525, 7, 10);

  diffConrtol(2512, 2526, 8, 9, 4, 5);
  diffConrtol(2512, 2526, 10, 11, 6, 7);
  diffConrtol(2512, 2526, 12, 13, 8, 9);
  
  diffConrtol(2512, 2526, 15, 16, 10, 11);
  diffConrtol(2513, 2526, 1, 2, 12, 13);
  
  normConrtol(2513, 2533, 3, 4);
  //- 55
  diffConrtol(2516, 2528, 0, 1, 0, 1);
  diffConrtol(2516, 2528, 2, 3, 2, 3);
  
  normConrtol(2516, 2527, 4, 7);
  normConrtol(2516, 2527, 5, 8);
  normConrtol(2516, 2527, 6, 9);
  normConrtol(2516, 2527, 7, 10);

  diffConrtol(2516, 2528, 8, 9, 4, 5);
  diffConrtol(2516, 2528, 10, 11, 6, 7);
  diffConrtol(2516, 2528, 12, 13, 8, 9);
  
  diffConrtol(2516, 2528, 15, 16, 10, 11);
  diffConrtol(2517, 2528, 1, 2, 12, 13);
  
  normConrtol(2517, 2533, 3, 5);
  //- 56
  diffConrtol(2520, 2532, 0, 1, 0, 1);
  diffConrtol(2520, 2532, 2, 3, 2, 3);
  
  normConrtol(2520, 2531, 4, 7);
  normConrtol(2520, 2531, 5, 8);
  normConrtol(2520, 2531, 6, 9);
  normConrtol(2520, 2531, 7, 10);

  diffConrtol(2520, 2532, 8, 9, 4, 5);
  diffConrtol(2520, 2532, 10, 11, 6, 7);
  diffConrtol(2520, 2532, 12, 13, 8, 9);
  
  diffConrtol(2520, 2532, 15, 16, 10, 11);
  diffConrtol(2521, 2532, 1, 2, 12, 13);
}

void diffConrtol(uint16_t inNum, uint16_t outNum, uint8_t fbit, uint8_t sbit, uint8_t ofbit, uint8_t osbit)
{
  if((PSW[inNum + fbit/16] & (1 << (fbit%16))) && !(PSW[inNum + sbit/16] & (1 << (sbit%16))))
  {
    PSW[outNum + ofbit/16] |= (1 << (ofbit%16));
    PSW[outNum + osbit/16] &= ~(1 << (osbit%16));
  }
  else if(!(PSW[inNum + fbit/16] & (1 << (fbit%16))) && (PSW[inNum + sbit/16] & (1 << (sbit%16))))
  {
    PSW[outNum + ofbit/16] &= ~(1 << (ofbit%16));
    PSW[outNum + osbit/16] &= ~(1 << (osbit%16));
  }
  else
  {
    PSW[outNum + ofbit/16] &= ~(1 << (ofbit%16));
    PSW[outNum + osbit/16] |= (1 << (osbit%16));
  }
}

void normConrtol(uint16_t inNum, uint16_t outNum, uint8_t bit, uint8_t obit)
{
  if((PSW[inNum + bit/16] & (1 << (bit%16))))
  {
    PSW[outNum + obit/16] |= (1 << (obit%16));
  }
  else
  {
    PSW[outNum + obit/16] &= ~(1 << (obit%16));
  }
}
