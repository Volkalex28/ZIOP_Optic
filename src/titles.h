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
  {alPowerOn,               "Включение панели"},
  {alOpenUserAccess,        "Доступ пользователя открыт"},
  {alOpenAdminAccess,       "Доступ администратора открыт"},
  {alConFailPanel1,         "Обрыв связи с АРМ1"},
  {alConFailPanel2,         "Обрыв связи с АРМ2"},
  {alConFailPanel3,         "Обрыв связи с АРМ3"},
  {alConFailPanel4,         "Обрыв связи с АРМ4"},
  {alConFailDP1,            "Обрыв связи с К-1"},
  {alConFailDP2,            "Обрыв связи с К-2"},
  {alConFailDP3,            "Обрыв связи с К-Т1"},
  {alConFailDP4,            "Обрыв связи с К-Т3"},
  {alConFailDP5,            "Обрыв связи с К-Т2"},
  {alConFailDP6,            "Обрыв связи с К-Т4"},
  {alConFailGate,           "Обрыв связи со Шлюзом 485-RTU-LAN"},
  {alNone}
};

#endif // __TITLES_H__