/**
 * @file alarms.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief ������������ ���� ������ ��� ������ � �������� � ���������
 * 
 */

/**
 * @defgroup Alarms ��������� � ��������� ���������
 * @ingroup Core
 * 
 * ������ ������ � ���������� �����������, �������� � ���������
 */

#ifndef __ALARMS_H__
#define __ALARMS_H__

// includes ---------------------------------------------------------------------------------------
#include "lib.h"

#include "devices/devices.h"

#include "mem/manager.h"

// defines ----------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_Define ����������� 
 * @ingroup Alarms
 * @{
 */

#define FIRST_RR_ALARMS         700   ///< ������ ������� ��� ������ �������� ������
#define FIRST_RR_ALARMS_GATE    1200  ///< ������ ������� ������� ������ ��� ������, ������� ������ ����
#define COUNT_ALARMS            199   ///< ������������ ���������� ��������� �������� ������

#define FIRST_RR_EVENT          5000  ///< ������ ������� ����������������� ������ ������� �������
#define COUNT_EVENTS            1000  ///< ������������ ���������� �������

#define FIRST_RR_CONFCRASH      1000  ///< ������ ������� ����������������� ������ ����� �������

///@}

// macroses ---------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_Macros ������� 
 * @ingroup Alarms
 * @{
 */

/**
 * @brief ������ ���������� ��������� �� ���� �������
 */
#define NUMBER_RR_FOR_ONE_EVENT (CALC_COUNT_RR(EventByte_t))

/**
 * @brief ������ ���������� ��������� ��� ������ ���� ���������� �������
 */
#define NumberOFCrashes         (((alAll % 16) != 0) ? alAll / 16 + 1 : alAll / 16)

///@}

// enums ------------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_Enums ������������
 * @ingroup Alarms
 * @{
 */

/**
 * @brief ������������ ��� ������� ������ ������.
 * ��������� ������� ���������� ��� ���������� ������ ���������� �
 * ����������� ������
 */
enum EAlarms
{
  alarmsBacklog,    ///< ����� ���� �������� ������
  alarmsActual,     ///< ����� �������� ������, ������� �����������������
  alarmsSHOT,       ///< ����� �������� ������ ���������� �� ���� (����)
  alarmsSHSN,       ///< ����� �������� ������ ���������� �� ���� (����)
  alarmsSHSND,      ///< ����� �������� ������ ���������� �� ���-� (����)
  alarmsCount       ///< ���������� ������� �������� ������
};

///@}

// typedefs ---------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_Typedef ����������� ����� 
 * @ingroup Alarms
 * @{
 */

/**
 * @brief ������ �������, ������� ��������� � �������������� � ������
 */
typedef enum EAlarm
{
//------  ARM  ------
  alNone,                                   ///< ������� ������� (�� ��������������)
  alPowerOn1,                               ///< ��������� ������ 1
  alPowerOn2,                               ///< ��������� ������ 2
  alPowerOn3,                               ///< ��������� ������ 3
  alPowerOn4,                               ///< ��������� ������ 4
  alOpenUserAccess1,                        ///< �������� ������� ������������ � ������ 1
  alOpenUserAccess2,                        ///< �������� ������� ������������ � ������ 2
  alOpenUserAccess3,                        ///< �������� ������� ������������ � ������ 3
  alOpenUserAccess4,                        ///< �������� ������� ������������ � ������ 4
  alOpenAdminAccess1,                       ///< �������� ������� �������������� � ������ 1
  alOpenAdminAccess2,                       ///< �������� ������� �������������� � ������ 2
  alOpenAdminAccess3,                       ///< �������� ������� �������������� � ������ 3
  alOpenAdminAccess4,                       ///< �������� ������� �������������� � ������ 4

  alConFailPanel1,                          ///< ������ ���������� � ������� 1
  alConFailPanel2,                          ///< ������ ���������� � ������� 2
  alConFailPanel3,                          ///< ������ ���������� � ������� 3
  alConFailPanel4,                          ///< ������ ���������� � ������� 4
  alConFailDP1,                             ///< ������ ���������� � ������ ���������� �������� 1
  alConFailDP2,                             ///< ������ ���������� � ������ ���������� �������� 2
  alConFailDP3,                             ///< ������ ���������� � ������ ���������� �������� 3
  alConFailDP4,                             ///< ������ ���������� � ������ ���������� �������� 4
  alConFailDP5,                             ///< ������ ���������� � ������ ���������� �������� 5
  alConFailDP6,                             ///< ������ ���������� � ������ ���������� �������� 6
  alConFailGate,                            ///< ������ ���������� �� ������

  alConRestoredPanel1,                      ///< ���������� � ������� 1 �������������
  alConRestoredPanel2,                      ///< ���������� � ������� 2 �������������
  alConRestoredPanel3,                      ///< ���������� � ������� 3 �������������
  alConRestoredPanel4,                      ///< ���������� � ������� 4 �������������
  alConRestoredDP1,                         ///< ���������� � ������ ���������� �������� 1 �������������
  alConRestoredDP2,                         ///< ���������� � ������ ���������� �������� 2 �������������
  alConRestoredDP3,                         ///< ���������� � ������ ���������� �������� 3 �������������
  alConRestoredDP4,                         ///< ���������� � ������ ���������� �������� 4 �������������
  alConRestoredDP5,                         ///< ���������� � ������ ���������� �������� 5 �������������
  alConRestoredDP6,                         ///< ���������� � ������ ���������� �������� 6 �������������
  alConRestoredGate,                        ///< ���������� �� ������ �������������
  alEndArm,                           // ���������� ������� ������ ���

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
  alEndShort,                         // ���������� ������� ������ ���

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
  alEndShsn,                          // ���������� ������� ������ ���
  
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
  alEndShsnD,                         // ���������� ������� ������ ����

  alShortEn = alEndShsnD,
  alShsnEn,
  alShSnDEn,                       

  alTransTemperProtectT1 = 200,
  alTransTemperProtectT2,
  alTransTemperProtectT3,
  alTransTemperProtectT4,
  alAll                ///< ���������� ���� �������
} Alarm_t;

/**
 * @brief ��������� ������ �������, ������� ��������� � ����������������� ������
 * 
 */
typedef struct EventByte_s 
{
	uint8_t Min;      ///< ������
	uint8_t Hour;     ///< ���
	uint8_t Day;      ///< ����
	uint8_t Sec;      ///< �������
	uint8_t Year;     ///< ��� (+2000)
	uint8_t Month;    ///< �����
	uint16_t Event;   ///< ����� ������� �� @link Alarm_t @endlink
} EventByte_t;

/**
 * @brief ��������� ������ ������
 * 
 */
typedef struct Alarms_s
{
  uint16_t count;                 ///< ���������� �������� ������
  uint16_t buf[COUNT_ALARMS];     ///< ����� �������� ������
} Alarms_t;

/**
 * @brief ��������� ����� ��� ������ ��������� 
 */
typedef enum EAlarmsMasks
{
  alarmsMaskIndicator,  ///< ����� ������� �� ��������� ������
  alarmsMaskMessage,    ///< ����� ������ ��������� �� ������ "�������� ������"
  alarmsMaskEvent,      ///< ����� ������������ �������
  alarmsMaskCount       ///< ���������� ����� ��� ������ ���������
} AlarmsMask_t;

///@}

// variables --------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_ExVars ���������� ��������� �����
 * @ingroup Alarms
 * @{
 */

/**
 * @brief ������ ���������� �� ������ �������� ������.
 */
extern Alarms_t * Alarms[alarmsCount];

///@}

// functions --------------------------------------------------------------------------------------
/**
 * @defgroup Alarms_ExFunctions ������� ��������� �����
 * @ingroup Alarms
 * @{
 */

/**
 * @brief ������� ���������� ������ �������
 * 
 * @param NumberCrash ����� ���������
 */
void addEvent(Alarm_t NumberCrash);

/**
 * @brief ������� ������� �������
 * 
 */
void clearEvents(void);

/**
 * @brief ������� ���������� ����� ��������� ��� ��������� ���������
 * 
 * @param[in] NumberCrash ����� ���������
 */
void addCrash(Alarm_t NumberCrash);

/**
 * @brief ������� �������� ��������� ��� ��������� ���������
 * 
 * @param NumberCrash 
 */
void deleteCrash(Alarm_t NumberCrash);

/**
 * @brief ������� ��� ���������� ������ �������� ��������� ��� ��������� ���������.
 * � ���� ������� �������� @link alarmsBacklog @endlink �����, � �������� ����������� ����� 
 * @link alarmsMaskMessage @endlink. ����� ���� ����������� ����� @link alarmsActual @endlink
 */
void fillCrash(void);

/**
 * @brief ������� ������������� ��������� � ��������� ���������
 * 
 * @param alarmsBacklogPtr ��������� �� ������� ����� ������
 */
void initAlarms(void);

/**
 * @brief ������� ��� ��������� ������� ���������� ���������
 * 
 * @param typeMask ����� ���� ���������� �� ������� ���������� ���������
 * @param numberAlarm ����� ���������
 * @return ��������� �� ����������
 * @retval true ���������� ��������� 
 * @retval false ���������� ��� 
 */
bool_t isMasked(AlarmsMask_t typeMask, Alarm_t numberAlarm);


/**
 * @brief ������� ��� �������������� ���������� ������ �� ������ �� �����
 * 
 * @param numberShield ����� ���� ���������� �� ������� ���������� ���������
 * @param numberAlarm ����� ���������
 * @return ���������� ����������� ����� ������ �� ������ 
 * @retval Alarm_t ����������� �����
 * @retval alNone ������ �� ��������������
 */
Alarm_t convertionNumberAlarm(Shield_t numberShield, uint16_t numberAlarm);

/**
 * @brief ������� ��������� ����� ��� ���������
 * 
 * @param typeMask ����� ���� ���������� ������� ���������� ����������
 * @param numberAlarm ����� ���������
 * @param state ��������� ����� ������� ���������� ����������
 */
void setMask(AlarmsMask_t typeMask, Alarm_t numberAlarm, bool_t state);

///@}

#endif // __ALARMS_H__
