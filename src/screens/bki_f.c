
#include "screen.h"

#include "..\alarms.h"

#include "..\mem\panel.h"

void screenBkif(void)
{
  int i;

  static short Nfider, KalibrK, LevelR, En_Sekcion;
  static short U1Osn, U1Dop, R1Plus, R1Minus, U1Plus, U1Minus;
  static short Plus1, Minus1, Plus2, Minus2, Plus3, Minus3;
  
  Nfider = 14;
  KalibrK = 12;
  LevelR = 20;
  En_Sekcion = 0;
  U1Osn = 2400; 
  U1Dop = 2300; 
  R1Plus = 200; 
  R1Minus = 200;
  U1Plus = 2000;
  U1Minus = 2000;
  Plus1 = 100;
  Minus1 = 200;
  Plus2 = 300;
  Minus2 = 400;
  Plus3 = 500;
  Minus3 = 600;

  Screens->Bkif.Nfider     = &Nfider;
  Screens->Bkif.KalibrK    = &KalibrK;
  Screens->Bkif.LevelR     = &LevelR;
  Screens->Bkif.En_Sekcion = &En_Sekcion;

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

    Screens->Bkif._1 = &U1Osn; 
    Screens->Bkif._2 = &U1Dop; 
    Screens->Bkif._3 = &R1Plus; 
    Screens->Bkif._4 = &R1Minus;
    Screens->Bkif._5 = &U1Plus;
    Screens->Bkif._6 = &U1Minus;

    Screens->Bkif.Settings.Visible.SelectFider = 0;

    Screens->Bkif.Settings.Visible._1 = Screens->Bkif.Settings.Visible._2 = Screens->Bkif.Settings.Visible._3 = 1;
  }
  else if(Screens->Bkif.Settings.Select == 1) {

    for(i = 0; i < 3; i++) {
      Screens->Bkif.Settings.Title[i] = i + Screens->Bkif.Settings.SelectFider + 3;

      Screens->Bkif.Settings.title[i].Title[0] = 2;
      Screens->Bkif.Settings.title[i].Title[1] = 3;
      Screens->Bkif.Settings.title[i].dimention = 1;
    }

    Screens->Bkif._1 = &Plus1;
    Screens->Bkif._2 = &Minus1;
    Screens->Bkif._3 = &Plus2;
    Screens->Bkif._4 = &Minus2;
    Screens->Bkif._5 = &Plus3;
    Screens->Bkif._6 = &Minus3;

    Screens->Bkif.Settings.Visible.SelectFider = (*Screens->Bkif.Nfider > 3);

    Screens->Bkif.Settings.Visible._1 = (*Screens->Bkif.Nfider >= 1);
    Screens->Bkif.Settings.Visible._2 = (*Screens->Bkif.Nfider >= 2);
    Screens->Bkif.Settings.Visible._3 = (*Screens->Bkif.Nfider >= 3);
  }
  Screens->Bkif.Settings.Visible.Select = (*Screens->Bkif.Nfider != 0);
}