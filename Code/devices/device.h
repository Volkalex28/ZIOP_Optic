/**
 * @file device.h
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "../lib.h"

typedef enum ECom Port_t;
typedef enum EFoo { f6, f10 } Foo_t;

typedef struct DeviceEEP_s
{
  uint16_t adress;
  uint16_t mode;
  uint16_t subMode;
  uint16_t masterID;
  Port_t port;

} DeviceEEP_t;

typedef struct Request_s
{
  uint16_t * ptr;
  uint16_t first;
  uint16_t count;
} Request_t;

typedef struct DeviceCon_s
{
  BOOL status;
  Foo_t function;
  Request_t * requests;
  uint8_t requestSize;

  uint16_t * pBuff;
  uint16_t buffSize;
} DeviceCon_t;

#endif // __DEVICE_H__