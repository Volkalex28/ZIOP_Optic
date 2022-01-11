/**
 * @file titles.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __TITLES_H__
#define __TITLES_H__

#include "alarms.h"

#define FIRST_RR_TITLES     10000
#define COUNT_RR_ONE_TITLE  25

typedef struct Title_s
{
  Alarm_t number;
  const char * name;
} Title_t;

void finitTitle(void);

const Title_t arrTitles[] = {
  {alPowerOn1,               "��������� ������ �1"},
  {alPowerOn2,               "��������� ������ �2"},
  {alPowerOn3,               "��������� ������ �-�1/3"},
  {alPowerOn4,               "��������� ������ �-�1/4"},
  {alOpenUserAccess1,        "������ ������������ ������ �1 ������"},
  {alOpenUserAccess2,        "������ ������������ ������ �2 ������"},
  {alOpenUserAccess3,        "������ ������������ ������ �-�1/3 ������"},
  {alOpenUserAccess4,        "������ ������������ ������ �-�1/4 ������"},
  {alOpenAdminAccess1,       "������ �������������� ������ �1 ������"},
  {alOpenAdminAccess2,       "������ �������������� ������ �2 ������"},
  {alOpenAdminAccess3,       "������ �������������� ������ �-�1/3 ������"},
  {alOpenAdminAccess4,       "������ �������������� ������ �-�1/4 ������"},

  {alConFailPanel1,         "����� ����� � ������� �1"},
  {alConFailPanel2,         "����� ����� � ������� �2"},
  {alConFailPanel3,         "����� ����� � ������� �-�1/3"},
  {alConFailPanel4,         "����� ����� � ������� �-�2/4"},
  {alConFailDP1,            "����� ����� � ������������ �-1"},
  {alConFailDP2,            "����� ����� � ������������ �-2"},
  {alConFailDP3,            "����� ����� � ������������ �-�1"},
  {alConFailDP4,            "����� ����� � ������������ �-�3"},
  {alConFailDP5,            "����� ����� � ������������ �-�2"},
  {alConFailDP6,            "����� ����� � ������������ �-�4"},
  {alConFailGate,           "����� ����� �� ������ 485-RTU-LAN"},
  {alConFailShot,                "����� ����� � ���"},
  {alConFailShsn,                "����� ����� � ���"},
  {alConFailShsnD,               "����� ����� � ���-�"},

//---
  
  {alConRestoredPanel1,         "����� � ������� �1 �������������"},
  {alConRestoredPanel2,         "����� � ������� �2 �������������"},
  {alConRestoredPanel3,         "����� � ������� �-�1/3 �������������"},
  {alConRestoredPanel4,         "����� � ������� �-�2/4 �������������"},
  {alConRestoredDP1,            "����� � ������������ �-1 �������������"},
  {alConRestoredDP2,            "����� � ������������ �-2 �������������"},
  {alConRestoredDP3,            "����� � ������������ �-�1 �������������"},
  {alConRestoredDP4,            "����� � ������������ �-�3 �������������"},
  {alConRestoredDP5,            "����� � ������������ �-�2 �������������"},
  {alConRestoredDP6,            "����� � ������������ �-�4 �������������"},
  {alConRestoredGate,           "����� �� ������ 485-RTU-LAN �������������"},
  {alConRestoredShot,            "����� �� ��� �������������"},
  {alConRestoredShsn,            "����� �� ��� �������������"},
  {alConRestoredShsnD,           "����� �� ���-� �������������"},

//-------------------------------------------------------------------------------------
//--------------------------  SHOT  ---------------------------------------------------
//-------------------------------------------------------------------------------------
  {alShotCon_1,                 "��������� ������"},
  {alShotCon_2,                 "�������� ������� ��������������"},
  {alShotCon_3,                 "�������� ������� ������������"},
//-------------------------------------------------------------------------------------
  {alShotCon_4,                 "����� ����� � ���1 (ESI)"},
  {alShotCon_5,                 "����� ����� � ���2 (ESI)"},
  {alShotCon_6,                 "����� ����� � ������.������ 1"},
  {alShotCon_7,                 "����� ����� � �������.������ 1"},
  {alShotCon_8,                 "����� ����� � ���(�) 1"},
  {alShotCon_9,                 "����� ����� � �������� ����. 1"},
//-------------------------------------------------------------------------------------
  {alShotCon_10,                "����� � ���1 ������������� (ESI)"},
  {alShotCon_11,                "����� � ���2 ������������� (ESI)"},
  {alShotCon_12,                "����� � ������.������ 1 �������������"},
  {alShotCon_13,                "����� � �������.������ 1 �������������"},
  {alShotCon_14,                "����� � ���(�) 1 �������������"},
  {alShotCon_15,                "����� � �������� ����. 1 �������������"},
//-------------------------------------------------------------------------------------
  {alShotB118_1,                "������������� ���1 (ESI)"},
  {alShotB118_2,                "������������� ���2 (ESI)"},
//-------------------------------------------------------------------------------------
//������
  {alShotAnP_1,                 "���������� �� ����� �ESI U>235B, U<209B"},
  {alShotAnP_2,                 "������������� ���1 (UZ1)"},
  {alShotAnP_3,                 "������������� ���2 (UZ2)"},
  {alShotAnP_4,                 "Q3"},
  {alShotAnP_5,                 "���������� �� ����� �ESI U>245B, U<198B"},
  {alShotAnP_6,                 "Q5"},
  {alShotAnP_7,                 "������� ���������� �� � �����"},
  {alShotAnP_8,                 "Q7"},
//�����
  {alShotAnP_9,                 "\"�����\" � ���� ����������� ���� R�� < 20 ���"},
  {alShotAnP_10,                "��������������� ������: �������� ������ ��"},
  {alShotAnP_11,                "��������� ����. ����������� �������������� �����"},
  {alShotAnP_12,                "��������� ����. ����������� �����"},
  {alShotAnP_13,                "��������� ����. ����������� �� ������ ���1(���2)"},
  {alShotAnP_14,                "����������� ����� � ��"},
  {alShotAnP_15,                "���������� ���������� �� �����"},
  {alShotAnP_16,                "I7"},
//-------------------------------------------------------------------------------------
  {alShotDP_1,                  "I1"},
  {alShotDP_2,                  "I2"},
  {alShotDP_3,                  "I3"},
  {alShotDP_4,                  "I4"},
  {alShotDP_5,                  "I5"},
  {alShotDP_6,                  "I6"},
  {alShotDP_7,                  "������������� ����-�"},
  {alShotDP_8,                  "��������� ���������� ����������� �� ����� 2 ~380�"},
  {alShotDP_9,                  "��������� ���������� ����������� �� ����� 1 ~380�"},
  {alShotDP_10,                 "��������� ���������� ����������� �� ����� ��"},
  {alShotDP_11,                 "I11"},
  {alShotDP_12,                 "I12"},
  {alShotDP_13,                 "I13"},
  {alShotDP_14,                 "I14"},
  {alShotDP_15,                 "I15"},
  {alShotDP_16,                 "I16"},
  {alShotDP_17,                 "I17"},
  {alShotDP_18,                 "I18"},
  {alShotDP_19,                 "I19"},
  {alShotDP_20,                 "I20"},
  {alShotDP_21,                 "1QF1 �R�� < 20 ���"},
  {alShotDP_22,                 "1QF2 �R�� < 20 ���"},
  {alShotDP_23,                 "1QF3 �R�� < 20 ���"},
  {alShotDP_24,                  "1QF4 �R�� < 20 ���"},
  {alShotDP_25,                 "1QF5 �R�� < 20 ���"},
  {alShotDP_26,                 "1QF6 �R�� < 20 ���"},
  {alShotDP_27,                 "1QF7 �R�� < 20 ���"},
  {alShotDP_28,                 "1QF8 �R�� < 20 ���"},
  {alShotDP_29,                 "1QF9 �R�� < 20 ���"},
  {alShotDP_30,                 "1QF10 �R�� < 20 ���"},
  {alShotDP_31,                 "1QF11 �R�� < 20 ���"},
  {alShotDP_32,                 "1QF12 �R�� < 20 ���"},
  {alShotDP_33,                 "1QF13 �R�� < 20 ���"},
  {alShotDP_34,                 "1QF14 �R�� < 20 ���"},
  {alShotDP_35,                 "Q15"},
  {alShotDP_36,                 "Q16"},
  {alShotDP_37,                 "Q17"},
  {alShotDP_38,                 "Q18"},
  {alShotDP_39,                 "Q19"},
  {alShotDP_40,                 "Q20"},
//-------------------------------------------------------------------------------------
  {alShotBKIf_1,                 "1QF1 �R�� < 40 ���"},
  {alShotBKIf_2,                 "1QF2 �R�� < 40 ���"},
  {alShotBKIf_3,                 "1QF3 �R�� < 40 ���"},
  {alShotBKIf_4,                 "1QF4 �R�� < 40 ���"},
  {alShotBKIf_5,                 "1QF5 �R�� < 40 ���"},
  {alShotBKIf_6,                 "1QF6 �R�� < 40 ���"},
  {alShotBKIf_7,                 "1QF7 �R�� < 40 ���"},
  {alShotBKIf_8,                 "1QF8 �R�� < 40 ���"},
  {alShotBKIf_9,                 "1QF9 �R�� < 40 ���"},
  {alShotBKIf_10,                "1QF10 �R�� < 40 ���"},
  {alShotBKIf_11,                "1QF11 �R�� < 40 ���"},
  {alShotBKIf_12,                "1QF12 �R�� < 40 ���"},
  {alShotBKIf_13,                "1QF13 �R�� < 40 ���"},
  {alShotBKIf_14,                "1QF14 �R�� < 40 ���"},
//-------------------------------------------------------------------------------------
//--------------------------  SHSN  ---------------------------------------------------
//-------------------------------------------------------------------------------------
  {alShsnCon_1,                  "��������� ������"},
  {alShsnCon_2,                  "�������� ������� ��������������"},
  {alShsnCon_3,                  "�������� ������� ������������"},
//-------------------------------------------------------------------------------------
  {alShsnCon_4,                  "����� ����� � �������.������ 1"},
  {alShsnCon_5,                  "����� � �������.������ 1 �������������"},
  {alShsn_1,                     "���� 1 �������� ��������"},
  {alShsn_2,                     "���� 2 �������� ��������"},
  {alShsn_3,                     "���� 1 ~220� �������� ��������"},
  {alShsn_4,                     "���� 1 ~36� �������� ��������"},
  {alShsn_5,                     "���� 2 ~220� �������� ��������"},
  {alShsn_6,                     "���� 2 ~36� �������� ��������"},
  {alShsn_7,                     "���� 1. ������� ��� �������� ���������"},
  {alShsn_8,                     "���� 2. ������� ��� �������� ���������"},
  {alShsn_9,                     "I9"},
  {alShsn_10,                    "I10"},
  {alShsn_11,                    "���� 1 ~220� � ������"},
  {alShsn_12,                    "���� 2 ~220� � ������"},
  {alShsn_13,                    "���� 1 ~36� � ������"},
  {alShsn_14,                    "���� 2 ~36� � ������"},
  {alShsn_15,                    "���� 1 ~220� ��� ����������"},
  {alShsn_16,                    "���� 2 ~220� ��� ����������"},
  {alShsn_17,                    "���� 1 ~36� ��� ����������"},
  {alShsn_18,                    "���� 2 ~36� ��� ����������"},
  {alShsn_19,                    "��������� ���������� ����������� ������ ~220�"},
  {alShsn_20,                    "��������� ���������� ����������� ������ ~36�"},
//-------------------------------------------------------------------------------------
//--------------------------  SHSN-D  -------------------------------------------------
//-------------------------------------------------------------------------------------
  {alShsnDCon_1,                 "��������� ������"},
  {alShsnDCon_2,                 "�������� ������� ��������������"},
  {alShsnDCon_3,                 "�������� ������� ������������"},
//-------------------------------------------------------------------------------------
  {alShsnDCon_4,                 "����� ����� � �������.������ 1"},
  {alShsnDCon_5,                 "����� � �������.������ 1 �������������"},
  {alShsnD_1,                    "���� 1 �������� ��������"},
  {alShsnD_2,                    "���� 2 �������� ��������"},
  {alShsnD_3,                    "���� 1 ~220� �������� ��������"},
  {alShsnD_4,                    "���� 1 ~36� �������� ��������"},
  {alShsnD_5,                    "���� 2 ~220� �������� ��������"},
  {alShsnD_6,                    "���� 2 ~36� �������� ��������"},
  {alShsnD_7,                    "���� 1. ������� ��� �������� ���������"},
  {alShsnD_8,                    "���� 2. ������� ��� �������� ���������"},
  {alShsnD_9,                    "I9"},
  {alShsnD_10,                   "I10"},
  {alShsnD_11,                   "���� 1 ~220� � ������"},
  {alShsnD_12,                   "���� 2 ~220� � ������"},
  {alShsnD_13,                   "���� 1 ~36� � ������"},
  {alShsnD_14,                   "���� 2 ~36� � ������"},
  {alShsnD_15,                   "���� 1 ~220� ��� ����������"},
  {alShsnD_16,                   "���� 2 ~220� ��� ����������"},
  {alShsnD_17,                   "���� 1 ~36� ��� ����������"},
  {alShsnD_18,                   "���� 2 ~36� ��� ����������"},
  {alShsnD_19,                   "��������� ���������� ����������� ������ ~220�"},
  {alShsnD_20,                   "��������� ���������� ����������� ������ ~36�"},

  {alConFailAtAllPanel,          "������ �� � ����"},
  {alConFailHighLevel,           "��� ����� � ������� �������"},

  {alTransTemperProtectT1_140,   "����������� ��-�� �1 > 140"}, 
  {alTransTemperProtectT2_140,   "����������� ��-�� �2 > 140"}, 
  {alTransTemperProtectT3_140,   "����������� ��-�� �3 > 140"}, 
  {alTransTemperProtectT4_140,   "����������� ��-�� �4 > 140"}, 
  {alTransTemperProtectT1,       "������! ����������� ��-�� �1 > 155"}, 
  {alTransTemperProtectT2,       "������! ����������� ��-�� �2 > 155"}, 
  {alTransTemperProtectT3,       "������! ����������� ��-�� �3 > 155"}, 
  {alTransTemperProtectT4,       "������! ����������� ��-�� �4 > 155"}, 
  {alFailOpticalSwitchKO14,      "������������� ����������� ����������� ��14"}, 
  {alFailOpticalSwitchKO15,      "������������� ����������� ����������� ��15"}, 
  {alFailOpticalSwitchKO2,       "������������� ����������� ����������� ��2"}, 
  {alFailOpticalSwitchKO1,       "������������� ����������� ����������� ��1"}, 

  {alShortEn,                    "������ ���"},
  {alShsnEn,                     "������ ���"},
  {alShSnDEn,                    "������ ���-�"},

  {alNone}
};

#endif // __TITLES_H__