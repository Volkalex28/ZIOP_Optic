/**
 * @file manager.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief Заголовочный файл модуля для работы с памятью и портами
 * 
 * С помощью этого модуля осуществяется обмен с внешними устройствами 
 * посредством портов, а также чтение/запись во внутреннюю память панели
 */

/**
 * @defgroup Manager Менеджер данных
 * @ingroup Data
 * 
 * Модуль работы с данными (как из внутренней памяти, так и из памяти внешних устройств)
 */

#ifndef __MEM_MANAGER_H__
#define __MEM_MANAGER_H__

#include "../lib.h"

#define CAST_TO_U16(_REG_)    (*(uint16_t *)& ## _REG_)
#define CAST_TO_U32(_REG_)    (*(uint32_t *)& ## _REG_)
#define CAST_TO_PU16(_REG_)   (*(uint16_t **)& ## _REG_)

#define CALC_COUNT_RR(_OBJ_)  (sizeof( _OBJ_ ) / sizeof(uint16_t))

enum PSW_Registers {
  TIME_HOUR    = 4090,
  TIME_MIN,
  TIME_SEC,
  TIME_DAY,
  TIME_MONTH,
  TIME_YEAR
};

enum PSB_Register {
	B_ON,
	B_OFF,
	B_GOTO_WAKEUPSCREEN = 9,
	B_ACCES_1LEVEL = 60,
	B_ACCES_2LEVEL,
	B_ACCES_3LEVEL,
	B_ACCES_4LEVEL,
	B_ACCES_5LEVEL,
	B_ACCES_6LEVEL,
	B_ACCES_7LEVEL,
	B_ACCES_8LEVEL,
	B_ACCES_9LEVEL
};

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

/**
 * @brief 
 * 
 * @param cell 
 * @return cell_t 
 */
cell_t read(cell_t cell);

/**
 * @brief 
 * 
 * @param cell 
 * @return cell_t 
 */
cell_t write(cell_t cell);

/**
 * @brief 
 * 
 * @param cell 
 * @param count 
 * @param pvalues 
 * @return cell_t 
 */
cell_t reads(cell_t cell, uint16_t count, uint16_t * pvalues);

/**
 * @brief 
 * 
 * @param cell 
 * @param count 
 * @param pvalues 
 * @return cell_t 
 */
cell_t writes(cell_t cell, uint16_t count, uint16_t * pvalues);

/**
 * @brief Get the ptr object
 * 
 * @param ret 
 */
void get_ptr(cell_t * ret);

/**
 * @brief 
 * 
 */
void initMem();

void selectCircle(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);
void selectNormal(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);
void selectNormalBlock(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);

#endif // __MEM_MANAGER_H__