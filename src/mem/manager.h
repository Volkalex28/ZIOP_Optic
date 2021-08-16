/**
 * @file manager.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __MEM_MANAGER_H__
#define __MEM_MANAGER_H__

#include "../lib.h"

#define FIRST_RR_PANEL    256
#define COUNT_RR_PANEL    (500-FIRST_RR_PANEL)

#define FIRST_RR_EEP      256
#define FIRST_RR_FOR_EEP  2000
#define COUNT_RR_EEP      500

#define FIRST_RR_ALARMS   700

#define FIRST_RR_SCREEN   500

typedef enum EMemTypes {
  memPFW,
  memPSW,
  memPSB,
  portDownload,
  portPLC,
  net0,
  net1,
  net2,
  net3,
  net4,
  net5
} MemTypes_t;

enum EMemStatus {
  memStatusIncorrectData = -1,
  memStatusFAIL,
  memStatusOK
};

typedef struct cell_s
{
  MemTypes_t type;
  uint8_t adress;
  uint32_t number;

  uint16_t value;

  uint16_t * ptr;
  int8_t status;
} cell_t;

cell_t read(cell_t cell);
cell_t write(cell_t cell);
cell_t reads(cell_t cell, uint16_t count, uint16_t * pvalues);
cell_t writes(cell_t cell, uint16_t count, uint16_t * pvalues);

void get_ptr(cell_t * ret);

void mem_init();

cell_t cell(void);

#define CELL(...) (cell_t){__VA_ARGS__}

#endif // __MEM_MANAGER_H__