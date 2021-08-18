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

void title_finit(void);

const Title_t arrTitles[] = {
  {alPowerOn,               "Включение панели"},
  {alOpenUserAccess,        "Доступ пользователя открыт"},
  {alOpenAdminAccess,       "Доступ администратора открыт"},
  {alNone}
};

#endif // __TITLES_H__