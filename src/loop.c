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

void taskLoop()
{

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

    if(Panel->oldScreen != PSW[1]) 
    {
      Panel->flags.menuIsOpen = false;
      if (PSW[1] == 5)
      {
        Panel->ChooseDevice.ResetCrashList = 1;
      }
    }
    if(Panel->flags.menuIsOpen) OpenWindow(10, 600, 68); else CloseWindow(10);
  
    selectDeviceInCrashAndEvent();
    ReadConfigCrashAndEvent();
    FillCrash();

    Panel->oldScreen = PSW[1];
    updatePFW();
    time();
    Delay(20);
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

void selectNormalBlock(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down) 
{
  if(Up)   (*Select)+=6;
  if(Down) (*Select)-=6;

  if(*Select < Min) (*Select) = Min; 
  if(*Select > Max) (*Select) = Max;
}
