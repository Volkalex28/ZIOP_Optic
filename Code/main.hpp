/**
 * @file main.h
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief Заголовочный файл, который описывает глобальные константы и функции
 * Содержит глобальные константы, определения типов и функций, которые
 * используются во всей программе
 */
#pragma once

#include <stdint.h>
#include <string.h>

#include "lib.h"

#define TO_UINT16(_STRUCT_) *((uint16_t*)(&(_STRUCT_)))

#define FIRST_RR_LOGS 5000
#define NUMBERS_LOGS  1000

/**
 * @brief Описание служебныйх битов Панели
 */
enum PSB_Registers 
{
	B_ON,                         ///< Всегда в состоянии лог. единицы
	B_OFF,                        ///< Всегда в состоянии лог. нуля
	B_GOTO_WAKEUPSCREEN = 9,      ///< Панель в режиме "сна"
	B_ACCES_1LEVEL = 60,          ///< Доступ первого уровня открыт
	B_ACCES_2LEVEL,               ///< Доступ второго уровня открыт
	B_ACCES_3LEVEL,               ///< Доступ третьего уровня открыт
	B_ACCES_4LEVEL,               ///< Доступ четвертого уровня открыт
	B_ACCES_5LEVEL,               ///< Доступ пятого уровня открыт
	B_ACCES_6LEVEL,               ///< Доступ шестого уровня открыт
	B_ACCES_7LEVEL,               ///< Доступ седьмого уровня открыт
	B_ACCES_8LEVEL,               ///< Доступ восьмого уровня открыт
	B_ACCES_9LEVEL,               ///< Доступ девятого уровня открыт
};

enum RR_Speed_COM {
	DOWNL_SPEED = 20,
	DOWNL_BITS,
	DOWNL_STOPBITS,
	DOWNL_PARITY,
	DOWNL_SLAVEID,
	DOWNL_TIMEOUT,
	DOWNL_FUNC,

	PLC_SPEED = 30,
	PLC_BITS,
	PLC_STOPBITS,
	PLC_PARITY,
	PLC_SLAVEID,
	PLC_TIMEOUT,
	PLC_FUNC
};

// namespace time
// {
// struct time_t
// {
//   uint8_t Seconds;
//   uint8_t Minutes;
//   uint8_t Hours;
//   uint8_t Day;
//   uint8_t Month;
//   uint8_t Year;
// };
  
// time_t get(void);

// };

void getUserLevelAvtorisation(void);
void getAdminLevelAvtorisation(void);

void setUserLevelAvtorisation(bool state);
void setAdminLevelAvtorisation(bool state);

