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

void screenMnemotic(void)
{

  Screens->Mnemotic.Settings.RegimeScreen[0] = 0;   
  Screens->Mnemotic.Settings.RegimeScreen[1] = 0;   
  Screens->Mnemotic.Settings.RegimeScreen[2] = 1;   
  Screens->Mnemotic.Settings.RegimeScreen[3] = 1;   
  Screens->Mnemotic.Settings.RegimeScreen[4] = 2;   
  Screens->Mnemotic.Settings.RegimeScreen[5] = 2;   
  Screens->Mnemotic.Settings.RegimeScreen[6] = 3;   
  Screens->Mnemotic.Settings.RegimeScreen[7] = 0;   
  Screens->Mnemotic.Settings.RegimeScreen[8] = 4;   
  Screens->Mnemotic.Settings.RegimeScreen[9] = 1;   
  Screens->Mnemotic.Settings.RegimeScreen[10] = 5;   
  Screens->Mnemotic.Settings.RegimeScreen[11] = 2;   
  Screens->Mnemotic.Settings.RegimeScreen[12] = 6;   
  Screens->Mnemotic.Settings.RegimeScreen[13] = 0;   
  Screens->Mnemotic.Settings.RegimeScreen[14] = 7;   
  Screens->Mnemotic.Settings.RegimeScreen[15] = 1;   
  Screens->Mnemotic.Settings.RegimeScreen[16] = 8;   
  Screens->Mnemotic.Settings.RegimeScreen[17] = 2;   
  Screens->Mnemotic.Settings.RegimeScreen[18] = 9;   
  Screens->Mnemotic.Settings.RegimeScreen[19] = 0;   
  Screens->Mnemotic.Settings.RegimeScreen[20] = 10;   
  Screens->Mnemotic.Settings.RegimeScreen[21] = 1;   
  Screens->Mnemotic.Settings.RegimeScreen[22] = 11;   
  Screens->Mnemotic.Settings.RegimeScreen[23] = 2;   
  Screens->Mnemotic.Settings.RegimeScreen[24] = 12;   
  Screens->Mnemotic.Settings.RegimeScreen[25] = 3;   
  Screens->Mnemotic.Settings.RegimeScreen[26] = 4;   
  Screens->Mnemotic.Settings.RegimeScreen[27] = 5;   


  if (Screens->Mnemotic.Settings.Flags.EventOpenWindow)
  {
    Screens->Mnemotic.Settings.Temp = Screens->Mnemotic.Settings.TitleWindowNumber;
    
    if (Screens->Mnemotic.Settings.Flags.FlagVisible)
    {
      Screens->Mnemotic.Settings.RegimeWindow1 = Screens->Mnemotic.Settings.RegimeScreen[Screens->Mnemotic.Settings.TitleWindowNumber+12];
    }
    else
    {
      Screens->Mnemotic.Settings.RegimeWindow1 = Screens->Mnemotic.Settings.RegimeScreen[Screens->Mnemotic.Settings.TitleWindowNumber*2];
      Screens->Mnemotic.Settings.RegimeWindow2 = Screens->Mnemotic.Settings.RegimeScreen[Screens->Mnemotic.Settings.TitleWindowNumber*2+1];
    }

    Screens->Mnemotic.Settings.Flags.Led1 = true;
    Screens->Mnemotic.Settings.Flags.Led2 = false;
    Screens->Mnemotic.Settings.Flags.Led3 = true;

    Screens->Mnemotic.Settings.Flags.EventOpenWindow = false;
  }
}
