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

void titleInit(void);

const Title_t arrTitles[] = {
  {alPowerOn,               "��������� ������"},
  {alOpenUserAccess,        "������ ������������ ������"},
  {alOpenAdminAccess,       "������ �������������� ������"},
  {alConFailPanel1,         "����� ����� � ���1"},
  {alConFailPanel2,         "����� ����� � ���2"},
  {alConFailPanel3,         "����� ����� � ���3"},
  {alConFailPanel4,         "����� ����� � ���4"},
  {alConFailDP1,            "����� ����� � �-1"},
  {alConFailDP2,            "����� ����� � �-2"},
  {alConFailDP3,            "����� ����� � �-�1"},
  {alConFailDP4,            "����� ����� � �-�3"},
  {alConFailDP5,            "����� ����� � �-�2"},
  {alConFailDP6,            "����� ����� � �-�4"},
  {alConFailGate,           "����� ����� �� ������ 485-RTU-LAN"},
  {alNone}
};

#endif // __TITLES_H__