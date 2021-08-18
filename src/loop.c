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
  uint16_t oldScreen = 0;
  init();

  while(true)
  {
    clearRRScreens();
    switch (PSW[1])
    {
    // case  5: screenCCrash(); break;
    // case  6: screenZvu();    break;
    // case  7: screenBkif();   break;
    case  8: screenShot();   break;
    // case  9: screenShsn();   break;
    case 10: screenCrash();  break;
    case 11: screenEvent();  break;
    default:                 break;
    }
    fillRRScreens();

    if(oldScreen != PSW[1]) Panel->flags.menuIsOpen = false;
    if(Panel->flags.menuIsOpen) OpenWindow(10, 600, 68); else CloseWindow(10);

    oldScreen = PSW[1];
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
