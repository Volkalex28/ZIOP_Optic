/**
 * @file alarms.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief Файл исходного кода модуля для работы с авариями и событиями
 *
 * Содержит описание функций для добавления/удаления аварийных и системных
 * сообщений и событий. Обабатывается маскировка сообщений
 */


#include "alarms.h"

#include "mem/manager.h"
#include "mem/panel.h"
#include "mem/pfw.h"

// varibles ---------------------------------------------------------------------------------------
Alarms_t * Alarms[alarmsCount];

/**
 * @defgroup Alarms_Vars Локальные переменные
 * @ingroup Alarms
 * @{
 */

/**
 * @brief Массив указателей на буферы в куче, которые синхронизированные с энергонезависимой памятью
 */
uint16_t * AlarmsMasks[alarmsMaskCount];

///@}

// local functions --------------------------------------------------------------------------------
/**
 * @defgroup Alarms_Functions Локальные функции
 * @ingroup Alarms
 * @{
 */

/**
 * @brief Функция для получения регистра маскировки сообщений
 * 
 * @param typeMask Выбор типа маскировки который необходимо получить
 * @param numberAlarm Номер сообщения
 * @return Значение регистра маскировки
 */
uint16_t getMaskValue(AlarmsMask_t typeMask, Alarm_t numberAlarm);

///@}

void addEvent(Alarm_t number)
{
  size_t i;
  EventByte_t	EventS;
  Time_t * pTime;
  cell_t c;

  if(isMasked(alarmsMaskEvent, number)) return;

  if(PFW->N_Event >= COUNT_EVENTS) 
  {
    PFW->N_Event = 0;											
    PFW->CB++;
  }
    
  pTime = time();		
  EventS.Sec    = pTime->Sec;
  EventS.Min    = pTime->Min;
  EventS.Hour   = pTime->Hour;
  EventS.Day    = pTime->Day;
  EventS.Month  = pTime->Month;
  EventS.Year   = pTime->Year;
  EventS.Event  = number;

  c.type = memPFW; 
  c.number = FIRST_RR_EVENT + PFW->N_Event*NUMBER_RR_FOR_ONE_EVENT;
  writes(c, NUMBER_RR_FOR_ONE_EVENT, &CAST_TO_U16(EventS));

  PFW->N_Event++;

}

void clearEvets(void)
{
  size_t i;
  uint16_t temp[NUMBER_RR_FOR_ONE_EVENT];
  cell_t c; c.type = memPFW;

  PFW->CB = PFW->N_Event = 0;
  for(i = 0; i < COUNT_EVENTS; i++)
  {
    c.adress = FIRST_RR_EVENT + NUMBER_RR_FOR_ONE_EVENT*i;
    writes(c, NUMBER_RR_FOR_ONE_EVENT, temp);
  }
}

void addCrash(Alarm_t NumberCrash) 
{
	size_t i, n;

	if(Alarms[alarmsBacklog]->count == COUNT_ALARMS) return;

	for(i = 0; i < Alarms[alarmsBacklog]->count; i++) 
    if(Alarms[alarmsBacklog]->buf[i] == NumberCrash) 
      return;

	Alarms[alarmsBacklog]->buf[Alarms[alarmsBacklog]->count++] = NumberCrash;
	addEvent(NumberCrash);
  
  if (isMasked(alarmsMaskIndicator, NumberCrash)) 
    Panel->flags.cvitCrash = 0;
}

void fillCrash(void) 
{
	size_t i, n;

  Alarms[alarmsTemp]->count = 0;

  for(i = 0; i < Alarms[alarmsBacklog]->count; i++) 
  {
    if(isMasked(alarmsMaskMessage, Alarms[alarmsBacklog]->buf[i]) == false)
      Alarms[alarmsTemp]->buf[Alarms[alarmsTemp]->count++] = Alarms[alarmsBacklog]->buf[i];
  }

  for(i = 0; i < Alarms[alarmsTemp]->count; i++)
  {
    Alarms[alarmsActual]->buf[i] = Alarms[alarmsTemp]->buf[i];
  }
  Alarms[alarmsActual]->count = Alarms[alarmsTemp]->count;

  for(i = 0; i < Alarms[alarmsBacklog]->count; i++) 
  {
    if(isMasked(alarmsMaskIndicator, Alarms[alarmsBacklog]->buf[i])) 
    {
      Panel->flags.noneCrash = false;
      return;
    }
  }
  Panel->flags.noneCrash = true;
}

void deleteCrash(Alarm_t NumberCrash) 
{
	size_t i;

	if(Alarms[alarmsBacklog]->count == 0) return;

	for(i = 0; i < Alarms[alarmsBacklog]->count; i++) 
  {
		if(Alarms[alarmsBacklog]->buf[i] == NumberCrash) 
    {
			Alarms[alarmsBacklog]->count--;

			for(; i < Alarms[alarmsBacklog]->count; i++)
				Alarms[alarmsBacklog]->buf[i] = Alarms[alarmsBacklog]->buf[i+1];
			Alarms[alarmsBacklog]->buf[i] = 0;
		}
	}
}

void initAlarms(void)
{
  size_t i;
  
  Alarms[alarmsActual]  = (Alarms_t *)&PSW[FIRST_RR_ALARMS];
  Alarms[alarmsBacklog] = (Alarms_t *)Malloc(sizeof(Alarms_t)*sizeof(uint16_t));
  Alarms[alarmsTemp]    = (Alarms_t *)Malloc(sizeof(Alarms_t)*sizeof(uint16_t));
  
  for(i = 0; i < alarmsMaskCount; i++)
    AlarmsMasks[i] = (uint16_t *)Malloc(NumberOFCrashes*sizeof(uint16_t));
}

void readMaskMessages(void)
{
  size_t i;
  cell_t c; c.type = memPFW; 

  for(i = 0; i < alarmsMaskCount; i++)
  {
    c.number = FIRST_RR_CONFCRASH + NumberOFCrashes*i;// + Screens->ConfCrash.Settings.Offset[0]/16; 
    reads(c, NumberOFCrashes, AlarmsMasks[i]);  // Вкл/Выкл аварию + Маска аварий + Маска событий
  }
}

bool_t isMasked(AlarmsMask_t typeMask, Alarm_t numberAlarm)
{
  return (getMaskValue(typeMask, numberAlarm) & (1 << (numberAlarm % 16))) != 0 ? true : false;
}

void setMask(AlarmsMask_t typeMask, Alarm_t numberAlarm, bool_t state)
{
  cell_t c; 
  
  c.type = memPFW; 
  c.number = FIRST_RR_CONFCRASH + NumberOFCrashes*typeMask + numberAlarm/16;
  c.value = getMaskValue(typeMask, numberAlarm);

  if(state != isMasked(typeMask, numberAlarm))
    if(state == true)
      c.value |= (1 << (numberAlarm % 16));
    else
      c.value &= ~(1 << (numberAlarm % 16));

  write(c);
}

// local functions --------------------------------------------------------------------------------
uint16_t getMaskValue(AlarmsMask_t typeMask, Alarm_t numberAlarm)
{
  return AlarmsMasks[typeMask][numberAlarm/16];
}
