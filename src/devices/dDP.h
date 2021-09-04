/**
 * @file dDP.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __DEVICES_DDP_H__
#define __DEVICES_DDP_H__

#include "devices.h"

#define N_DP 6

DEF_DEVICE(DP, 1, 
  union DP_DIO_u
  {
    uint16_t regs[4];
    struct DP_IO_s
    {
      struct DP_I_s
      {
        uint32_t I0   : 1;
        uint32_t I1   : 1;
        uint32_t I2   : 1;
        uint32_t I3   : 1;
        uint32_t I4   : 1;
        uint32_t I5   : 1;
        uint32_t I6   : 1;
        uint32_t I7   : 1;
        uint32_t I8   : 1;
        uint32_t I9   : 1;
        uint32_t I10  : 1;
        uint32_t I11  : 1;
        uint32_t I12  : 1;
        uint32_t I13  : 1;
        uint32_t I14  : 1;
        uint32_t I15  : 1;
        uint32_t I16  : 1;
        uint32_t I17  : 1;
        uint32_t I18  : 1;
        uint32_t I19  : 1;
      } I;
      struct DP_O_s
      {
        uint32_t O0   : 1;
        uint32_t O1   : 1;
        uint32_t O2   : 1;
        uint32_t O3   : 1;
        uint32_t O4   : 1;
        uint32_t O5   : 1;
        uint32_t O6   : 1;
        uint32_t O7   : 1;
        uint32_t O8   : 1;
        uint32_t O9   : 1;
        uint32_t O10  : 1;
        uint32_t O11  : 1;
        uint32_t O12  : 1;
        uint32_t O13  : 1;
        uint32_t O14  : 1;
        uint32_t O15  : 1;
        uint32_t O16  : 1;
        uint32_t O17  : 1;
        uint32_t O18  : 1;
        uint32_t O19  : 1;
      } O;
    } IO;
    struct DP_I40_s
    {
      uint32_t I0   : 1;
      uint32_t I1   : 1;
      uint32_t I2   : 1;
      uint32_t I3   : 1;
      uint32_t I4   : 1;
      uint32_t I5   : 1;
      uint32_t I6   : 1;
      uint32_t I7   : 1;
      uint32_t I8   : 1;
      uint32_t I9   : 1;
      uint32_t I10  : 1;
      uint32_t I11  : 1;
      uint32_t I12  : 1;
      uint32_t I13  : 1;
      uint32_t I14  : 1;
      uint32_t I15  : 1;
      uint32_t I16  : 1;
      uint32_t I17  : 1;
      uint32_t I18  : 1;
      uint32_t I19  : 1;
      uint32_t I20  : 1;
      uint32_t I21  : 1;
      uint32_t I22  : 1;
      uint32_t I23  : 1;
      uint32_t I24  : 1;
      uint32_t I25  : 1;
      uint32_t I26  : 1;
      uint32_t I27  : 1;
      uint32_t I28  : 1;
      uint32_t I29  : 1;
      uint32_t I30  : 1;
      uint32_t I31  : 1;
      uint32_t I32  : 1;
      uint32_t I33  : 1;
      uint32_t I34  : 1;
      uint32_t I35  : 1;
      uint32_t I36  : 1;
      uint32_t I37  : 1;
      uint32_t I38  : 1;
      uint32_t I39  : 1;
    } I40;
  } DIO;
  
)

#endif // __DEVICES_DDP_H__