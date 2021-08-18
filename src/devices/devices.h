/**
 * @file devices.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __DEVICES_H__
#define __DEVICES_H__

#include "../mem/manager.h"

#define DEF_DEVICE(NAME, NREQ, ...) \
  typedef struct d##NAME##mem_s {   \
    __VA_ARGS__                     \
  } d##NAME##mem_t;                 \
  typedef struct d##NAME##_s {      \
    device_t instance;              \
    request_t reqs[NREQ];           \
    d##NAME##mem_t * mem;           \
  } d##NAME##_t;                    \
  void NAME##_finit(void);          \
  void NAME##_init(void);

typedef struct dmain_s
{
  MemTypes_t port;
  uint8_t adress;
  struct dmainFlags_s
  {
    uint8_t enabled   : 1;
    uint8_t mode      : 2;
  } flags;
} dmain_t;

typedef struct request_s
{
  uint16_t *  buf;
  uint16_t    first;
  uint8_t     count;
} request_t;


typedef struct device_s
{
  dmain_t *   main;
  request_t * reqs;
  uint8_t     nreqs;

  uint8_t     errCon  : 1;
} device_t;

void devices_finit();
void devices_init();

#endif // __DEVICES_H__