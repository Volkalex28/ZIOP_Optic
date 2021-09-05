/**
 * @file screen.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "string.h"

#include "screen.h"
#include "../mem/panel.h"
#include "..\devices\devices.h"

Screens_t * Screens;

void selectDeviceInCrashAndEvent(void)
{
  selectCircle(&Panel->ChooseDevice.Select, shieldShsnD, shieldARM, \
    Panel->ChooseDevice.EventBut.SelectUp, Panel->ChooseDevice.EventBut.SelectDown);

  switch (Panel->ChooseDevice.Select)
  {
    case shieldARM:    strcpy(Panel->ChooseDevice.TitleCh, "юпл");   break;
    case shieldShot:  strcpy(Panel->ChooseDevice.TitleCh, "ьнр");   break;
    case shieldShsn:   strcpy(Panel->ChooseDevice.TitleCh, "ьям");   break;
    case shieldShsnD:  strcpy(Panel->ChooseDevice.TitleCh, "ьям-д"); break;
    default: strcpy(Panel->ChooseDevice.TitleCh, " "); break;
  }

  if (Panel->ChooseDevice.EventBut.SelectUp || Panel->ChooseDevice.EventBut.SelectDown)
    Panel->ChooseDevice.ResetCrashList = 1;

  Panel->ChooseDevice.EventBut.SelectUp = Panel->ChooseDevice.EventBut.SelectDown = false;
}

void initScreens(void)
{
  Screens = (Screens_t *)&PSW[FIRST_RR_SCREEN];
}

void clearRRScreens(void)
{
  size_t i;
  
  for(i = 0; i < 50; i++)
  {
    PSW[FIRST_RR_SCREEN + i] = 0;
    if(Panel->oldScreen != PSW[CURRENT_SCREEN])
      PSW[FIRST_RR_SCREEN + 50 + i] = 0;
    CAST_TO_PU16(PSW[FIRST_RR_SCREEN + 100 + i*2]) = NULL;
  }
}

void fillRRScreens(void)
{
  size_t i;

  if(PSW[CURRENT_SCREEN] == scrEvent)
    return;

  for(i = 0; i < 50; i++)
  {
    PSW[FIRST_RR_SCREEN + i] = CAST_TO_PU16(PSW[FIRST_RR_SCREEN + 100 + i*2]) != NULL 
      ? *CAST_TO_PU16(PSW[FIRST_RR_SCREEN + 100 + i*2]) 
      : 0;
  }
}

void updateScreen(void)
{
  if(Panel->oldScreen != PSW[CURRENT_SCREEN]) 
  {
    Panel->flags.menuIsOpen = false;
    if (PSW[CURRENT_SCREEN] == scrConfAlarms)
      Panel->ChooseDevice.ResetCrashList = 1;
  }

  Panel->oldScreen = PSW[CURRENT_SCREEN];
}

void controlMenu(void)
{
  if(Panel->flags.menuIsOpen) 
  {
    if(GetAdminLevelAvtorisation == false)
    {
      CloseWindow(winMenuSyst);
      OpenWindow(winMenu, 600, 68); 
    }
    else
    {
      OpenWindow(winMenu,  600, 68);
      OpenWindow(winMenuSyst, 400, 68);
    }
  }
  else 
  {
    CloseWindow(winMenu);
    CloseWindow(winMenuSyst);
  }
}
