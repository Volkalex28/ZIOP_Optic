/**
 * @file loop.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "lib.h"
#include "init.h"
#include "screens/screen.h"

#include "mem/panel.h"
#include "mem/pfw.h"

Screens_t * Screens;

void normConrtol(uint16_t inNum, uint16_t outNum, uint8_t bit, uint8_t obit);
void diffConrtol(uint16_t inNum, uint16_t outNum, uint8_t fbit, uint8_t sbit, uint8_t ofbit, uint8_t osbit);

void taskLoop()
{
//  uint16_t oldScreen = 0;
  uint16_t count[6];
  int i;
  init();

  while(true)
  {
    clearRRScreens();
    switch (PSW[1])
    {
    case  5: screenConfCrash(); break;
    case  6: screenZvu();    break;
    case  7: screenBkif();   break;
    case  8: screenShot();   break;
    case  9: screenShsn();   break;
    case 10: screenCrash();  break;
    case 11: screenEvent();  break;
    default:                 break;
    }
    fillRRScreens();

    if(Panel->oldScreen != PSW[1]) Panel->flags.menuIsOpen = false;
    if(Panel->flags.menuIsOpen) OpenWindow(10, 600, 68); else CloseWindow(10);
  
    selectDeviceInCrashAndEvent();

    if(GetPSBStatus(300 + i) == false) 
    {
      cell_t c; c.type = memPFW; c.number = 2064 + i*16;
      c = read(c);

      // if(GetPSBStatus(54 + i) == false)
        SetPSB(300 + i);
      // else 
      // {
      //   if(c.value & 0x2)
      //     c.value &= ~0x2;
      //   else
      //     c.value |= 0x2;
        
        // write(c);
      // }

    }
    i++;
    i %= 3;

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
    //---

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


    Panel->oldScreen = PSW[1];
    updatePFW();
    time();
    Delay(50);
  }
}

void selectCircle(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down) 
{
  if(Up)   (*Select)++;
  if(Down) (*Select)--;

  if(*Select < Min) (*Select) = Max; 
  if(*Select > Max) (*Select) = Min;
}

void selectNormal(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down) 
{
  if(Up)   (*Select)++;
  if(Down) (*Select)--;

  if(*Select < Min) (*Select) = Min; 
  if(*Select > Max) (*Select) = Max;
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
