
#include "screen.h"

#include "..\alarms.h"
#include "..\devices\devices_mem.h"
#include "..\mem\panel.h"

void screenBkif(void)
{
  size_t i;

  Screens->Bkif.Nfider     = &dMem->Gate->SHOT.BKI.Nfider;
  Screens->Bkif.KalibrK    = &dMem->Gate->SHOT.BKI.CalibrK;
  Screens->Bkif.LevelR     = &dMem->Gate->SHOT.BKI.LevelR;
  Screens->Bkif.En_Sekcion = &dMem->Gate->SHOT.BKI.En_Section;

  selectCircle(&Screens->Bkif.Settings.Select, *Screens->Bkif.Nfider >= 1, 0, \
    Screens->Bkif.Settings.Event.SelectUp, Screens->Bkif.Settings.Event.SelectDown);
  Screens->Bkif.Settings.Event.SelectUp = Screens->Bkif.Settings.Event.SelectDown = false;

  selectCircle(&Screens->Bkif.Settings.SelectFider, (*Screens->Bkif.Nfider >= 3) ? *Screens->Bkif.Nfider - 3 : 0, 0, \
    Screens->Bkif.Settings.Event.FiderUp, Screens->Bkif.Settings.Event.FiderDown);
  Screens->Bkif.Settings.Event.FiderUp = Screens->Bkif.Settings.Event.FiderDown = false;

  if(Screens->Bkif.Settings.Select == 0) {
    for(i = 0; i < 3; i++) {
      Screens->Bkif.Settings.Title[i] = i;
    }

    Screens->Bkif.Settings.title[0].Title[0] = 0;
    Screens->Bkif.Settings.title[0].Title[1] = 1;
    Screens->Bkif.Settings.title[0].dimention = 0;
    
    Screens->Bkif.Settings.title[1].Title[0] = 2;
    Screens->Bkif.Settings.title[1].Title[1] = 3;
    Screens->Bkif.Settings.title[1].dimention = 1;
    
    Screens->Bkif.Settings.title[2].Title[0] = 2;
    Screens->Bkif.Settings.title[2].Title[1] = 3;
    Screens->Bkif.Settings.title[2].dimention = 0;

    Screens->Bkif.values[0] = &dMem->Gate->SHOT.BKI.U1Osn; 
    Screens->Bkif.values[1] = &dMem->Gate->SHOT.BKI.U1Dop; 
    Screens->Bkif.values[2] = &dMem->Gate->SHOT.BKI.R1Plus; 
    Screens->Bkif.values[3] = &dMem->Gate->SHOT.BKI.R1Minus;
    Screens->Bkif.values[4] = &dMem->Gate->SHOT.BKI.U1Plus;
    Screens->Bkif.values[5] = &dMem->Gate->SHOT.BKI.U1Minus;

    Screens->Bkif.Settings.Visible.SelectFider = 0;

    Screens->Bkif.Settings.Visible._1 = Screens->Bkif.Settings.Visible._2 = Screens->Bkif.Settings.Visible._3 = 1;
  }
  else if(Screens->Bkif.Settings.Select == 1) {

    for(i = 0; i < 3; i++) {
      Screens->Bkif.Settings.Title[i] = i + Screens->Bkif.Settings.SelectFider + 3;

      Screens->Bkif.Settings.title[i].Title[0] = 2;
      Screens->Bkif.Settings.title[i].Title[1] = 3;
      Screens->Bkif.Settings.title[i].dimention = 1;

      Screens->Bkif.values[2*i+0] = &dMem->Gate->SHOT.BKI.Fider[Screens->Bkif.Settings.SelectFider+i].Plus;
      Screens->Bkif.values[2*i+1] = &dMem->Gate->SHOT.BKI.Fider[Screens->Bkif.Settings.SelectFider+i].Minus;
    }

    Screens->Bkif.Settings.Visible.SelectFider = (*Screens->Bkif.Nfider > 3);

    Screens->Bkif.Settings.Visible._1 = (*Screens->Bkif.Nfider >= 1);
    Screens->Bkif.Settings.Visible._2 = (*Screens->Bkif.Nfider >= 2);
    Screens->Bkif.Settings.Visible._3 = (*Screens->Bkif.Nfider >= 3);
  }
  Screens->Bkif.Settings.Visible.Select = (*Screens->Bkif.Nfider != 0);
}