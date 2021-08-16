/**
 * @file screen.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "screen.h"
#include "../mem/panel.h"

Screens_t * Screens;

void selectDeviceInCrashAndEvent(void)
{
  selectCircle(&Panel->ChooseDevice.Select, 3, 0, \
    Panel->ChooseDevice.EventBut.SelectUp, Panel->ChooseDevice.EventBut.SelectDown);
  Panel->ChooseDevice.EventBut.SelectUp = Panel->ChooseDevice.EventBut.SelectDown = false;
}
