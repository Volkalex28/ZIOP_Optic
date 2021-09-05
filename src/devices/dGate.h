/**
 * @file dGate.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __DGATE_H__
#define __DGATE_H__

#include "devices.h"

#include "dDP.h"

#include "../mem/panel.h"

#define N_Gate 1

typedef struct GateSettEvent_s
{
  uint16_t N_Event;
  uint16_t CB;
} GateSettEvent_t;

DEF_DEVICE(Gate, 1, 
  struct SHOT_s 
  {
    struct B118_s	
    {
      uint16_t Uout;
      int16_t Iout;
      uint16_t Imax;
      uint16_t Umax;
      uint16_t Umin;
      struct B118_State_s 
      {
        uint16_t State    : 1;
        uint16_t Mode     : 1;
        uint16_t Protect  : 1;
        uint16_t Crash    : 1;
      } State;
      uint16_t Ufloat;
      uint16_t Boost;
    } ZVU[2];
    
    struct AnP_s {
      struct AnP_DIO_s 
      {
        uint16_t Q0 : 1;
        uint16_t Q1 : 1;
        uint16_t Q2 : 1;
        uint16_t Q3 : 1;
        uint16_t Q4 : 1;
        uint16_t Q5 : 1;
        uint16_t Q6 : 1;
        uint16_t Q7 : 1;

        uint16_t I0 : 1;
        uint16_t I1 : 1;
        uint16_t I2 : 1;
        uint16_t I3 : 1;
        uint16_t I4 : 1;
        uint16_t I5 : 1;
        uint16_t I6 : 1;
        uint16_t I7 : 1;
      } DIO;
      uint16_t K1;
      uint16_t K2;
      uint16_t K3;
      uint16_t K11;
      uint16_t K12;
      uint16_t K13;
      uint16_t SetZero;
      uint16_t TempPVT10;
      uint16_t ErrConPVT10;
      uint16_t _34;
      int16_t Umin1;
      int16_t Umax1;
      uint16_t Umin2;
      uint16_t Umax2;
      uint16_t Umin3;
      int16_t AI1;
      int16_t AI2;
      int16_t AI3;
      int16_t AI11;
      int16_t AI12;
      int16_t AI13;
    } AnP;

    dDPmem_t DP;

    struct BKI_s
    {
      int16_t U1Osn;
      int16_t U1Dop;
      uint16_t _2;
      uint16_t _3;
      int16_t R1Plus;
      int16_t R1Minus;
      int16_t U1Plus;
      int16_t U1Minus;
      uint16_t _8;
      uint16_t _9;
      uint16_t _10;
      uint16_t _11;
      uint16_t _12;
      uint16_t _13;
      uint16_t _14;
      uint16_t _15;
      struct BKI_Fider_s 
      {
        int16_t Plus;
        int16_t Minus;
      } Fider[32];

      int16_t Nfider;
      int16_t CalibrK;
      int16_t _150;
      int16_t _151;
      int16_t LevelR;
      int16_t En_Section;
    } BKI;

    Time_t time;
  } SHOT;

  struct SHSN_s
  {
    dDPmem_t  DP;
    Time_t    time;
  } SHSN[2];

  struct GateSHOT_ConfPanel_s
  {
    uint16_t WriteProtect : 1;
    uint16_t Autorisation : 1;
    uint16_t TempComp     : 1;
  } SHOT_ConfPanel;
  GateSettEvent_t SettEvent[3];

  uint16_t SHOT_CrashUminAnP;
  uint16_t SHOT_CrashUmaxAnP;
  uint16_t SHOT_ZVU_OldU;
  uint16_t SHOT_Ubanok;
  uint16_t SHOT_KTemperature;
  uint16_t SHOT_ZVU_UoutTemperature;
  uint16_t SHOT_ZVU_Nbanok;

  uint16_t SHOT_ZVU_Uzad[2];
  uint16_t SHOT_ZVU_Regime;
  uint16_t SHOT_ZVU_State[2];

  struct ErrConGateSlaves_s
  {
    uint16_t SHOT   : 1;
    uint16_t SHSN   : 1;
    uint16_t SHSND  : 1;
  } errCon;
)

#endif // __DGATE_H__