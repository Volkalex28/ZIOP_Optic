/**
 * @file titles.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "titles.h"
#include "mem/manager.h"
  
const Title_t arrTitles[] = {
  {alPowerOn,               "Включение панели"},
  {alOpenUserAccess,        "Доступ пользователя открыт"},
  {alOpenAdminAccess,       "Доступ администратора открыт  "},
  {alNone}
};

void titleInit(void)
{
  int i = 0;
  uint16_t temp[COUNT_RR_ONE_TITLE] = {0};

  for(i = 0; arrTitles[i].number != alNone; i++)
  {
    cell_t c; 
    c.type = memPFW; 
    c.number = FIRST_RR_TITLES + (arrTitles[i].number - 1) * COUNT_RR_ONE_TITLE;

    snprintf((char *)temp, COUNT_RR_ONE_TITLE * 2, "%s", arrTitles[i].name);
    writes(c, COUNT_RR_ONE_TITLE, temp);
  }
}
