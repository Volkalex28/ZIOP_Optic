/**
 * @file manager.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief ???????????? ???? ?????? ??? ?????? ? ??????? ? ???????
 * 
 * ? ??????? ????? ?????? ????????????? ????? ? ???????? ???????????? 
 * ??????????? ??????, ? ????? ??????/?????? ?? ?????????? ?????? ??????
 */

/**
 * @defgroup Manager ???????? ??????
 * @ingroup Data
 * 
 * ?????? ?????? ? ??????? (??? ?? ?????????? ??????, ??? ? ?? ?????? ??????? ?????????)
 */

#ifndef __MEM_MANAGER_H__
#define __MEM_MANAGER_H__

// includes ---------------------------------------------------------------------------------------
#include "../lib.h"

// macroses ---------------------------------------------------------------------------------------
/**
 * @defgroup Manager_Macros ??????? 
 * @ingroup Manager
 * @{
 */

/**
 * @brief ?????????????? ?????????? _VAR_ ? ??? uint16_t 
 */
#define CAST_TO_U16(_VAR_)    (*(uint16_t *)& ## _VAR_)
/**
 * @brief ?????????????? ?????????? _VAR_ ? ??? uint32_t 
 */
#define CAST_TO_U32(_VAR_)    (*(uint32_t *)& ## _VAR_)
/**
 * @brief ?????????????? ?????????? _VAR_ ? ????????? ?? ??? uint16_t 
 */
#define CAST_TO_PU16(_VAR_)   (*(uint16_t **)& ## _VAR_)
/**
 * @brief ?????? ?????????? ?????????, ??????? ???????? ?????? _OBJ_
 */
#define CALC_COUNT_RR(_OBJ_)  (sizeof( _OBJ_ ) / sizeof(uint16_t))

///@}

// enums ------------------------------------------------------------------------------------------
/**
 * @defgroup Manager_Enums ????????????
 * @ingroup Manager
 * @{
 */

/**
 * @brief ???????? ??????? PSB
 * 
 * ?????????? ???????? ??????????? ?????? (?????)
 */
enum PSB_Register 
{
	B_ON,                           ///< ?????? ?????????? ???
	B_OFF,                          ///< ?????? ?????????? ???

	B_GOTO_WAKEUPSCREEN   = 9,      ///< ??? ????????? ?????????
  
	B_ACCES_1LEVEL        = 60,     ///< ??? ???????? ??????? ?????? 1
	B_ACCES_2LEVEL,                 ///< ??? ???????? ??????? ?????? 2
	B_ACCES_3LEVEL,                 ///< ??? ???????? ??????? ?????? 3
	B_ACCES_4LEVEL,                 ///< ??? ???????? ??????? ?????? 4
	B_ACCES_5LEVEL,                 ///< ??? ???????? ??????? ?????? 5
	B_ACCES_6LEVEL,                 ///< ??? ???????? ??????? ?????? 6
	B_ACCES_7LEVEL,                 ///< ??? ???????? ??????? ?????? 7
	B_ACCES_8LEVEL,                 ///< ??? ???????? ??????? ?????? 8
	B_ACCES_9LEVEL                  ///< ??? ???????? ??????? ?????? 9
};

/**
 * @brief ???????? ??????? PSW
 * 
 * ????????????????? ???????? ??????????? ??????
 */
enum PSW_Registers 
{
  CURRENT_SCREEN  = 1,        ///< ??????? ?????

  TIME_YEAR_HEX   = 30,       ///< ?? ??????????????? ??????? ??????? (???)
  TIME_MONTH_HEX,             ///< ?? ??????????????? ??????? ??????? (?????)
  TIME_DAY_HEX,               ///< ?? ??????????????? ??????? ??????? (????)
  TIME_HOUR_HEX,              ///< ?? ??????????????? ??????? ??????? (????)
  TIME_MIN_HEX,               ///< ?? ??????????????? ??????? ??????? (??????)
  TIME_SEC_HEX,               ///< ?? ??????????????? ??????? ??????? (???????)

  TIME_HOUR       = 4090,     ///< ??????????????? ??????? ??????? (????)
  TIME_MIN,                   ///< ??????????????? ??????? ??????? (??????)
  TIME_SEC,                   ///< ??????????????? ??????? ??????? (???????)
  TIME_DAY,                   ///< ??????????????? ??????? ??????? (????)
  TIME_MONTH,                 ///< ??????????????? ??????? ??????? (?????)
  TIME_YEAR                   ///< ??????????????? ??????? ??????? (???)
};

/**
 * @brief ???????? ??????? PFW
 * 
 * ????????????????? ???????? ????????????????? ??????
 */
enum PFW_Registers
{
  PFW_IP_ADRESS_4   = 87    ///< ????????? ????? ???????????? IP ??????
};

///@}

// typedefs ---------------------------------------------------------------------------------------
/**
 * @defgroup Manager_Typedef ??????????? ????? 
 * @ingroup Manager
 * @{
 */

/**
 * @brief ??? ?????? ??? ??????? ??????/?????? ?????? 
 */
typedef enum EMemStatus 
{
  memStatusIncorrectData = -1,  ///< ??????? ?????????? ?????? ?? ????? ??????
  memStatusFAIL,                ///< ?????? ?????? ???????
  memStatusOK                   ///< ??????? ??????????? ?????
} MemStatus_t;

/**
 * @brief ??? ?????? ??? ??????? ???????????? ????? 
 */
typedef enum EMemTypes 
{
  memPFW,         ///< ????????????????? ?????? ??????
  memPSW,         ///< ??????????? ?????? (????????)
  memPSB,         ///< ??????????? ?????? (?????)
  portDownload,   ///< ?????? ???? Download
  portPLC,        ///< ?????? ???? PLC
  net0,           ///< ???? Ethernet ?? IP ?????? 1
  net1,           ///< ???? Ethernet ?? IP ?????? 2
  net2,           ///< ???? Ethernet ?? IP ?????? 3
  net3,           ///< ???? Ethernet ?? IP ?????? 4
  net4,           ///< ???? Ethernet ?? IP ?????? 5
  net5            ///< ???? Ethernet ?? IP ?????? 6
} MemTypes_t;

/**
 * @brief ??? ?????? ?????? ??? ???????/???????? ?????? ??????? 
 */
typedef struct cell_s
{
  MemTypes_t    type;     ///< ??? ?????? ??? ??????
  uint8_t       adress;   ///< ????? ???????? ?????????? (ID)
  uint32_t      number;   ///< ????? ???????? ??? ??????

  uint16_t      value;    ///< ???????? ??? ?????? ??? ????? ?????? ????????

  uint16_t *    ptr;      ///< ????????? ?? ?????? ?????? ???? ??? ????? ????? ??? NULL
  int8_t        status;   ///< ?????? ??????
} cell_t;

///@}

// functions --------------------------------------------------------------------------------------
/**
 * @defgroup Manager_ExFunctions ??????? ????????? ?????
 * @ingroup Manager
 * @{
 */

/**
 * @brief ??????? ??? ?????? ?????? ????????.
 * 
 * @param cell ???????? ????????, ??????? ?????????? ?????????
 * @return ???????? ???????????? ???????? ? ??????? ?????? 
 */
cell_t read(cell_t cell);

/**
 * @brief ??????? ??? ?????? ?????? ????????.
 * 
 * @param cell ???????? ????????, ??????? ?????????? ????????
 * @return ???????? ??????????? ???????? ? ??????? ??????  
 */
cell_t write(cell_t cell);

/**
 * @brief ??????? ??? ?????? ?????????? ?????????.
 * 
 * @param cell ???????? ?????????? ????????, ?? ???????? ?????????? ?????? ??????
 * @param count ?????????? ????????? ??? ??????
 * @return ???????? ?????????? ???????? ??? ?????? ? ??????? ??????  
 */
cell_t reads(cell_t cell, uint16_t count);

/**
 * @brief ??????? ??? ?????? ?????????? ?????????.
 * 
 * @param cell ???????? ?????????? ????????, ?? ???????? ?????????? ?????? ??????
 * @param count ?????????? ????????? ??? ??????
 * @return ???????? ?????????? ???????? ??? ?????? ? ??????? ?????? 
 */
cell_t writes(cell_t cell, uint16_t count);

/**
 * @brief ????????? ????????? ?? ????????? ???????.
 * ???? ????????? ?????? ???????? - NULL
 * 
 * @param[in, out] ret ???????? ??????? ??? ????????? ?????????
 */
void get_ptr(cell_t * ret);

/**
 * @brief ??????? ????????????? ?????? ??????.
 */
void initMem();

/**
 * @brief ??????? ?????????????/????????????? ???????? \p Select ? ?????????????.
 * ???? ???????? \p Select ????? ???????? ?????? \p Max ?? \p Select ?????????????? ? \p Min .
 * ???? ???????? \p Select ????? ???????? ?????? \p Min ?? \p Select ?????????????? ? \p Max .
 * 
 * @param[in, out] Select ????????? ?? ???????
 * @param Max ???????????? ???????? ????????
 * @param Min ??????????? ???????? ????????
 * @param Up ??????? ?????????? ?????????????
 * @param Down ??????? ?????????? ?????????????
 */
void selectCircle(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);

/**
 * @brief ??????? ?????????????/????????????? ???????? \p Select ? ?????????? ?? ??????? ?????????.
 * ???? ???????? \p Select ????? ???????? ?????? \p Max ?? \p Select ?????????????? ? \p Max .
 * ???? ???????? \p Select ????? ???????? ?????? \p Min ?? \p Select ?????????????? ? \p Min .
 * 
 * @param[in, out] Select ????????? ?? ???????
 * @param Max ???????????? ???????? ????????
 * @param Min ??????????? ???????? ????????
 * @param Up ??????? ?????????? ?????????????
 * @param Down ??????? ?????????? ?????????????
 */
void selectNormal(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down);

/**
 * @brief ??????? ?????????????/????????????? ?? \p Offset ???????? \p Select ? ?????????? ?? ??????? ?????????.
 * ???? ???????? \p Select ????? ???????? ?????? \p Max ?? \p Select ?????????????? ? \p Max .
 * ???? ???????? \p Select ????? ???????? ?????? \p Min ?? \p Select ?????????????? ? \p Min .
 * 
 * @param[in, out] Select ????????? ?? ???????
 * @param Offset ?? ??????? ????????? ???????? ???????
 * @param Max ???????????? ???????? ????????
 * @param Min ??????????? ???????? ????????
 * @param Up ??????? ?????????? ?????????????
 * @param Down ??????? ?????????? ?????????????
 */
void selectNormalBlock(int16_t * Select, uint16_t Offset, int16_t Max, int16_t Min, bool_t Up, bool_t Down);

///@}

#endif // __MEM_MANAGER_H__