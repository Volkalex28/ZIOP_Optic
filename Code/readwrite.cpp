/**
 * @file readwrite.cpp
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "readwrite.hpp"
#include "main.hpp"

// namespace RW
// {

// namespace psb 
// {
// bool read(uint16_t adr) 
// {
//   return GetPSBStatus(adr) > 0;
// }
// void write(uint16_t adr, bool value) 
// {
//   if(value == true)
//     SetPSB(adr);
//   else
//     ResetPSB(adr);
// }
// }

// namespace psw
// {
uint16_t read(uint16_t adr) 
{
  return PSW[adr];
}
void read(uint16_t adr, uint16_t num, uint16_t * pArr) 
{
  for(size_t i = 0; i < num; i++)
  {
    pArr[i] = PSW[adr + i];
  }
}
void write(uint16_t adr, uint16_t value) 
{
  PSW[adr] = value;
}
void write(uint16_t adr, uint16_t num, uint16_t * pArr) 
{
  for(size_t i = 0; i < num; i++)
  {
    PSW[adr + i] = pArr[i];
  }
}
// }

// namespace pfw
// {
// uint16_t read(uint16_t adr) 
// {
//   uint16_t temp = 0;
//   Read(HMI_LOCAL_MCH, 0, TYPE_PFW, adr, 0, TYPE_WORD, &temp);
//   return temp;
// }
// void read(uint16_t adr, uint16_t num, uint16_t * pArr) 
// {
//   Reads(HMI_LOCAL_MCH, 0, TYPE_PFW, adr, num, pArr);
// }
// void write(uint16_t adr, uint16_t value) 
// {
//   Write(HMI_LOCAL_MCH, 0, TYPE_PFW, adr, 0, TYPE_WORD, value);
// }
// void write(uint16_t adr, uint16_t num, uint16_t * pArr) 
// {
//   Writes(HMI_LOCAL_MCH, 0, TYPE_PFW, adr, num, pArr);
// }
// }

// namespace port
// {
// void setFunction(ID port, Function fun) {
//     RR_Speed_COM RegFun = (port == ID::download) ? RR_Speed_COM::DOWNL_FUNC : RR_Speed_COM::PLC_FUNC;
//     pfw::write(RegFun, (fun == port::Function::F16) ? pfw::read(RegFun)|(1<<2) : pfw::read(RegFun)&~(1<<2));
// }
// uint8_t getIDPort(ID port)
// {
//   switch (port)
//   {
//   case ID::plc: return PLC; break;
//   case ID::download: return DOWNLOAD; break;
//   }
// }

// bool read(ID port, uint8_t id_device, uint16_t adr, uint16_t * pValue) 
// {
//   return Read(getIDPort(port), id_device, MODBUS_RTU_REG_4X, adr, 0, TYPE_WORD, pValue);
// }
// bool read(ID port, uint8_t id_device, uint16_t adr, uint16_t num, uint16_t * pArr) 
// {
//   return Reads(getIDPort(port), id_device, MODBUS_RTU_REGS_4X, adr, num, pArr);
// }
// bool write(ID port, Function foo, uint8_t id_device, uint16_t adr, uint16_t value) 
// {
//   setFunction(port, foo);
//   return Write(getIDPort(port), id_device, MODBUS_RTU_REG_4X, adr, 0, TYPE_WORD, value);
// }
// bool write(ID port, Function foo, uint8_t id_device, uint16_t adr, uint16_t num, uint16_t * pArr) 
// {
//   if(foo == Function::F16)
//   {
//     setFunction(port, foo);
//     return Writes(getIDPort(port), id_device, MODBUS_RTU_REGS_4X, adr, num, pArr);
//   }
//   else
//   {
//     bool result = false;
//     for(size_t i = 0; i < num; i++)
//     {
//       if(write(port, foo, id_device, adr, pArr[i]) == true)
//       {
//         result = true;
//       }
//     }
//     return result;
//   }
// }
// }

// }
