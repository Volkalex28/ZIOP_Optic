/**
 * @file alarms.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief Заголовочный файл модуля для работы с авариями и событиями
 * 
 */

/**
 * @defgroup Alarms Аварийные и системные сообщения
 * @ingroup Core
 * 
 * Модуль работы с системными сообщениями, авариями и событиями
 */

#ifndef __ALARMS_H__
#define __ALARMS_H__

// includes ---------------------------------------------------------------------------------------
#include "lib.h"

#include "devices/devices.h"

#include "mem/manager.h"

// defines ----------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_Define Определения 
 * @ingroup Alarms
 * @{
 */

#define FIRST_RR_ALARMS         700   ///< Первый регистр для буфера активных аварий
#define FIRST_RR_ALARMS_GATE    1200  ///< Первый регистр буферов аварий для шкафов, которые читает шлюз
#define COUNT_ALARMS            199   ///< Максимальное количество возможных активных аварий

#define FIRST_RR_EVENT          5000  ///< Первый регистр энергонезависимой памяти журнала событий
#define COUNT_EVENTS            1000  ///< Максимальное количество событий

#define FIRST_RR_CONFCRASH      1000  ///< Первый регистр энергонезависимой памяти маски событий

///@}

// macroses ---------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_Macros Макросы 
 * @ingroup Alarms
 * @{
 */

/**
 * @brief Расчет количества регистров на одно событие
 */
#define NUMBER_RR_FOR_ONE_EVENT (CALC_COUNT_RR(EventByte_t))

/**
 * @brief Расчет количества регистров для одного типа маскировки событий
 */
#define NumberOFCrashes         (((alAll % 16) != 0) ? alAll / 16 + 1 : alAll / 16)

///@}

// enums ------------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_Enums Перечисления
 * @ingroup Alarms
 * @{
 */

/**
 * @brief Перечисление для буферов аварий панели.
 * Несколько буферов необходимо для корректной работы маскировки и
 * отображения аварий
 */
enum EAlarms
{
  alarmsBacklog,    ///< Буфер всех активных аварий
  alarmsActual,     ///< Буфер активных аварий, которые незамаскированные
  alarmsSHOT,       ///< Буфер активных аварий полученных из ШОТа (шлюз)
  alarmsSHSN,       ///< Буфер активных аварий полученных из ШСНа (шлюз)
  alarmsSHSND,      ///< Буфер активных аварий полученных из ШСН-Д (шлюз)
  alarmsCount       ///< Количество буферов активных аварий
};

///@}

// typedefs ---------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_Typedef Определения типов 
 * @ingroup Alarms
 * @{
 */

/**
 * @brief Номера событий, которые возникают и обрабатываются в панели
 */
typedef enum EAlarm
{
//------  ARM  ------
  alNone,                                   ///< Нулевое событие (не обрабатывается)
  alPowerOn1,                               ///< Включение панели 1
  alPowerOn2,                               ///< Включение панели 2
  alPowerOn3,                               ///< Включение панели 3
  alPowerOn4,                               ///< Включение панели 4
  alOpenUserAccess1,                        ///< Открытие доступа пользователя в панели 1
  alOpenUserAccess2,                        ///< Открытие доступа пользователя в панели 2
  alOpenUserAccess3,                        ///< Открытие доступа пользователя в панели 3
  alOpenUserAccess4,                        ///< Открытие доступа пользователя в панели 4
  alOpenAdminAccess1,                       ///< Открытие доступа администратора в панели 1
  alOpenAdminAccess2,                       ///< Открытие доступа администратора в панели 2
  alOpenAdminAccess3,                       ///< Открытие доступа администратора в панели 3
  alOpenAdminAccess4,                       ///< Открытие доступа администратора в панели 4

  alConFailPanel1,                          ///< Ошибка соединения с панелью 1
  alConFailPanel2,                          ///< Ошибка соединения с панелью 2
  alConFailPanel3,                          ///< Ошибка соединения с панелью 3
  alConFailPanel4,                          ///< Ошибка соединения с панелью 4
  alConFailDP1,                             ///< Ошибка соединения с платой дискретных сигналов 1
  alConFailDP2,                             ///< Ошибка соединения с платой дискретных сигналов 2
  alConFailDP3,                             ///< Ошибка соединения с платой дискретных сигналов 3
  alConFailDP4,                             ///< Ошибка соединения с платой дискретных сигналов 4
  alConFailDP5,                             ///< Ошибка соединения с платой дискретных сигналов 5
  alConFailDP6,                             ///< Ошибка соединения с платой дискретных сигналов 6
  alConFailGate,                            ///< Ошибка соединения со шлюзом

  alConRestoredPanel1,                      ///< Соединение с панелью 1 ввостановлено
  alConRestoredPanel2,                      ///< Соединение с панелью 2 ввостановлено
  alConRestoredPanel3,                      ///< Соединение с панелью 3 ввостановлено
  alConRestoredPanel4,                      ///< Соединение с панелью 4 ввостановлено
  alConRestoredDP1,                         ///< Соединение с платой дискретных сигналов 1 ввостановлено
  alConRestoredDP2,                         ///< Соединение с платой дискретных сигналов 2 ввостановлено
  alConRestoredDP3,                         ///< Соединение с платой дискретных сигналов 3 ввостановлено
  alConRestoredDP4,                         ///< Соединение с платой дискретных сигналов 4 ввостановлено
  alConRestoredDP5,                         ///< Соединение с платой дискретных сигналов 5 ввостановлено
  alConRestoredDP6,                         ///< Соединение с платой дискретных сигналов 6 ввостановлено
  alConRestoredGate,                        ///< Соединение со шлюзом ввостановлено
  alEndArm,                           // Количество событий секции АРМ

//------  SHOT  ------
  alShotCon_1 = alEndArm,
  alShotCon_2,
  alShotCon_3,
  alShotCon_4,
  alShotCon_5,
  alShotCon_6,
  alShotCon_7,
  alShotCon_8,
  alShotCon_9,
  alShotCon_10,
  alShotCon_11,
  alShotCon_12,
  alShotCon_13,
  alShotCon_14,
  alShotCon_15,
  alShotB118_1,
  alShotB118_2,
  alShotAnP_1,
  alShotAnP_2,
  alShotAnP_3,
  alShotAnP_4,
  alShotAnP_5,
  alShotAnP_6,
  alShotAnP_7,
  alShotAnP_8,
  alShotAnP_9,
  alShotAnP_10,
  alShotAnP_11,
  alShotAnP_12,
  alShotAnP_13,
  alShotAnP_14,
  alShotAnP_15,
  alShotAnP_16,
  alShotDP_1,
  alShotDP_2,
  alShotDP_3,
  alShotDP_4,
  alShotDP_5,
  alShotDP_6,
  alShotDP_7,
  alShotDP_8,
  alShotDP_9,
  alShotDP_10,
  alShotDP_11,
  alShotDP_12,
  alShotDP_13,
  alShotDP_14,
  alShotDP_15,
  alShotDP_16,
  alShotDP_17,
  alShotDP_18,
  alShotDP_19,
  alShotDP_20,
  alShotDP_21,
  alShotDP_22,
  alShotDP_23,
  alShotDP_24,
  alShotDP_25,
  alShotDP_26,
  alShotDP_27,
  alShotDP_28,
  alShotDP_29,
  alShotDP_30,
  alShotDP_31,
  alShotDP_32,
  alShotDP_33,
  alShotDP_34,
  alShotDP_35,
  alShotDP_36,
  alShotDP_37,
  alShotDP_38,
  alShotDP_39,
  alShotDP_40,
  alShotBKIf_1,
  alShotBKIf_2,
  alShotBKIf_3,
  alShotBKIf_4,
  alShotBKIf_5,
  alShotBKIf_6,
  alShotBKIf_7,
  alShotBKIf_8,
  alShotBKIf_9,
  alShotBKIf_10,
  alShotBKIf_11,
  alShotBKIf_12,
  alShotBKIf_13,
  alShotBKIf_14,
  alEndShort,                         // Количество событий секции ШОТ

//------  SHSN  ------
  alShsnCon_1 = alEndShort,
  alShsnCon_2,
  alShsnCon_3,
  alShsnCon_4,
  alShsnCon_5,
  alShsn_1,  
  alShsn_2,  
  alShsn_3,  
  alShsn_4,  
  alShsn_5,  
  alShsn_6,  
  alShsn_7,  
  alShsn_8,  
  alShsn_9,  
  alShsn_10,  
  alShsn_11,  
  alShsn_12,  
  alShsn_13,  
  alShsn_14,  
  alShsn_15,  
  alShsn_16,  
  alShsn_17,  
  alShsn_18,  
  alShsn_19,  
  alShsn_20,
  alEndShsn,                          // Количество событий секции ШСН
  
//------  SHSN-D  ------
  alShsnDCon_1 = alEndShsn,
  alShsnDCon_2,
  alShsnDCon_3,
  alShsnDCon_4,
  alShsnDCon_5,
  alShsnD_1,  
  alShsnD_2,  
  alShsnD_3,  
  alShsnD_4,  
  alShsnD_5,  
  alShsnD_6,  
  alShsnD_7,  
  alShsnD_8,  
  alShsnD_9,  
  alShsnD_10,  
  alShsnD_11,  
  alShsnD_12,  
  alShsnD_13,  
  alShsnD_14,  
  alShsnD_15,  
  alShsnD_16,  
  alShsnD_17,  
  alShsnD_18,  
  alShsnD_19,  
  alShsnD_20,
  alEndShsnD,                         // Количество событий секции ШСНД

  alShortEn = alEndShsnD,
  alShsnEn,
  alShSnDEn,                       

  alTransTemperProtectT1 = 200,
  alTransTemperProtectT2,
  alTransTemperProtectT3,
  alTransTemperProtectT4,
  alAll                ///< Количество всех событий
} Alarm_t;

/**
 * @brief Структура одного события, которое храниется в энергонезависимой памяти
 * 
 */
typedef struct EventByte_s 
{
	uint8_t Min;      ///< Минута
	uint8_t Hour;     ///< Час
	uint8_t Day;      ///< День
	uint8_t Sec;      ///< Секунда
	uint8_t Year;     ///< Год (+2000)
	uint8_t Month;    ///< Месяц
	uint16_t Event;   ///< Номер события из @link Alarm_t @endlink
} EventByte_t;

/**
 * @brief Структура буфера аварий
 * 
 */
typedef struct Alarms_s
{
  uint16_t count;                 ///< Количество активных аварий
  uint16_t buf[COUNT_ALARMS];     ///< Буфер активных аварий
} Alarms_t;

/**
 * @brief Возможные маски для одного сообщения 
 */
typedef enum EAlarmsMasks
{
  alarmsMaskIndicator,  ///< Маска влияния на индикатор аварий
  alarmsMaskMessage,    ///< Маска вывода сообщений на экране "Просмотр аварий"
  alarmsMaskEvent,      ///< Маска формирования события
  alarmsMaskCount       ///< Количество масок для одного сообщения
} AlarmsMask_t;

///@}

// variables --------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_ExVars Переменные доступные извне
 * @ingroup Alarms
 * @{
 */

/**
 * @brief Массив указателей на буфера активных аварий.
 */
extern Alarms_t * Alarms[alarmsCount];

///@}

// functions --------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_ExFunctions Функции доступные извне
 * @ingroup Alarms
 * @{
 */

/**
 * @brief Функция добавления нового события
 * 
 * @param NumberCrash Номер сообщения
 */
void addEvent(Alarm_t NumberCrash);

/**
 * @brief Функция очистки событий
 * 
 */
void clearEvents(void);

/**
 * @brief Функция добавления новых аварийных или системных сообщений
 * 
 * @param[in] NumberCrash Номер сообщения
 */
void addCrash(Alarm_t NumberCrash);

/**
 * @brief Функция удаления аварийных или системных сообщений
 * 
 * @param NumberCrash 
 */
void deleteCrash(Alarm_t NumberCrash);

/**
 * @brief Функция для заполнения буфера активных аварийных или системных сообщений.
 * В этой функции читается @link alarmsBacklog @endlink буфер, к которому применяется маска 
 * @link alarmsMaskMessage @endlink. После чего обновляется буфер @link alarmsActual @endlink
 */
void fillCrash(void);

/**
 * @brief Функция инициализации аварийных и системных сообщений
 * 
 * @param alarmsBacklogPtr Указатель на теневой буфер аварий
 */
void initAlarms(void);

/**
 * @brief Функция для получения статуса маскировки сообщения
 * 
 * @param typeMask Выбор типа маскировки на который необходимо проверить
 * @param numberAlarm Номер сообщения
 * @return Применена ли маскировка
 * @retval true Маскировка применена 
 * @retval false Маскировки нет 
 */
bool_t isMasked(AlarmsMask_t typeMask, Alarm_t numberAlarm);


/**
 * @brief Функция для ковертирования полученных аварий по шкафам из шлюза
 * 
 * @param numberShield Выбор типа маскировки на который необходимо проверить
 * @param numberAlarm Номер сообщения
 * @return Возвращает приведенный номер аварии из шкафов 
 * @retval Alarm_t Приведенный номер
 * @retval alNone Аварии не поддерживаются
 */
Alarm_t convertionNumberAlarm(Shield_t numberShield, uint16_t numberAlarm);

/**
 * @brief Функция установки маски для сообщения
 * 
 * @param typeMask Выбор типа маскировки который необходимо установить
 * @param numberAlarm Номер сообщения
 * @param state Состояние маски которое необходимо установить
 */
void setMask(AlarmsMask_t typeMask, Alarm_t numberAlarm, bool_t state);

///@}

#endif // __ALARMS_H__
