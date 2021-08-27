/**
 * @file screen.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "string.h"

#include "screen.h"
#include "../mem/panel.h"

Screens_t * Screens;

void selectDeviceInCrashAndEvent(void)
{
  selectCircle(&Panel->ChooseDevice.Select, 3, 0, \
    Panel->ChooseDevice.EventBut.SelectUp, Panel->ChooseDevice.EventBut.SelectDown);

  switch (Panel->ChooseDevice.Select)
  {
    case 0:  strcpy(Panel->ChooseDevice.TitleCh, "юпл");   break;
    case 1:  strcpy(Panel->ChooseDevice.TitleCh, "ьнр");   break;
    case 2:  strcpy(Panel->ChooseDevice.TitleCh, "ьям");   break;
    case 3:  strcpy(Panel->ChooseDevice.TitleCh, "ьям-д"); break;
    default: strcpy(Panel->ChooseDevice.TitleCh, " "); break;
  }

  if (Panel->ChooseDevice.EventBut.SelectUp || Panel->ChooseDevice.EventBut.SelectDown)
    Panel->ChooseDevice.ResetCrashList = 1;

  Panel->ChooseDevice.EventBut.SelectUp = Panel->ChooseDevice.EventBut.SelectDown = false;
}
