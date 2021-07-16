/**
 * @file readwrite.hpp
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "lib.h"

// namespace RW
// {

// namespace psb
// {
//   static bool read(uint16_t adr);
//   static void write(uint16_t adr, bool value);
// }

// namespace psw
// {
  static uint16_t read(uint16_t adr);
  static void read(uint16_t adr, uint16_t num, uint16_t * pArr);
  static void write(uint16_t adr, uint16_t value);
  static void write(uint16_t adr, uint16_t num, uint16_t * pArr);
// }

// namespace pfw
// {
//   static uint16_t read(uint16_t adr);
//   static void read(uint16_t adr, uint16_t num, uint16_t * pArr);
//   static void write(uint16_t adr, uint16_t value);
//   static void write(uint16_t adr, uint16_t num, uint16_t * pArr);
// }

// namespace port
// {
//   enum class ID { plc, download };
//   enum class Function { F6, F16 };

//   static bool read(ID port, uint8_t id_device, uint16_t adr, uint16_t * pValue);
//   static bool read(ID port, uint8_t id_device, uint16_t adr, uint16_t num, uint16_t * pArr);
//   static bool write(ID port, Function foo, uint8_t id_device, uint16_t adr, uint16_t value);
//   static bool write(ID port, Function foo, uint8_t id_device, uint16_t adr, uint16_t num, uint16_t * pArr);
// }

// }
