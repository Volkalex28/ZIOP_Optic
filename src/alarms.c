/**
 * @file alarms.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief Файл исходного кода модуля для работы с авариями и событиями
 *
 * Содержит описание функций для добавления/удаления аварийных и системных
 * сообщений и событий. Обабатывается маскировка сообщений
 */

#include "stdlib.h"

#include "alarms.h"

#include "mem/manager.h"
#include "mem/panel.h"
#include "mem/pfw.h"

// varibles ---------------------------------------------------------------------------------------
Alarms_t * Alarms[alarmsCount];

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
  addEventTime(number, *getTime());
}

void addEventTime(Alarm_t number, Time_t time)
{
  size_t i;
  EventByte_t	EventS;
  cell_t c;

  if(alPowerOn1 <= number && number <= alPowerOn4)
    PFW->statePanelsEvents[0][number - alPowerOn1] = time;
  if(alOpenUserAccess1 <= number && number <= alOpenUserAccess4)
    PFW->statePanelsEvents[1][number - alOpenUserAccess1] = time;
  if(alOpenAdminAccess1 <= number && number <= alOpenAdminAccess4)
    PFW->statePanelsEvents[2][number - alOpenAdminAccess1] = time;

  if(isMasked(alarmsMaskEvent, number) || (Panel->flags.isMaster == false)) return;

  if(PFW->N_Event >= COUNT_EVENTS) 
  {
    PFW->N_Event = 0;											
    PFW->CB++;
  }
    
  EventS.Sec    = time.Sec;
  EventS.Min    = time.Min;
  EventS.Hour   = time.Hour;
  EventS.Day    = time.Day;
  EventS.Month  = time.Month;
  EventS.Year   = time.Year;
  EventS.Event  = number;

  c.type    = memPFW; 
  c.number  = FIRST_RR_EVENT + PFW->N_Event*NUMBER_RR_FOR_ONE_EVENT;
  c.ptr     = &CAST_TO_U16(EventS);
  writes(c, NUMBER_RR_FOR_ONE_EVENT);

  PFW->N_Event++;
}

void clearEvents(bool_t gate)
{
  int i, n;
  uint16_t zero_buf[NUMBER_RR_FOR_ONE_EVENT] = {0};

  PFW->CB = PFW->N_Event = 0;
  for(i = 0; i < COUNT_EVENTS; i++)
  {
    cell_t c; c.type = memPFW; c.number = FIRST_RR_EVENT + i*NUMBER_RR_FOR_ONE_EVENT; c.ptr = zero_buf;
    writes(c, NUMBER_RR_FOR_ONE_EVENT);
  }

  if(gate) for(n = 0; n < 3; n++)
  {
    PFW->gateSettEvent[n].CB = PFW->gateSettEvent[n].N_Event = 0;
    for(i = 0; i < COUNT_EVENTS; i++)
    {
      cell_t c; c.type = memPFW; c.number = FIRST_RR_EVENT_GATE + i*NUMBER_RR_FOR_ONE_EVENT; c.ptr = zero_buf;
      writes(c, NUMBER_RR_FOR_ONE_EVENT);
    }
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
  
  if (isMasked(alarmsMaskIndicator, NumberCrash) == false) 
    Panel->flags.cvitCrash = false;
}

void fillCrash(void) 
{
	size_t i, n;


  if(Panel->flags.isMaster == true) 
    Alarms[alarmsActual]->count = 0;

  // if(Panel->flags.isMaster == true) 
  for(i = 0; i < Alarms[alarmsBacklog]->count; i++) 
  {
    if(isMasked(alarmsMaskMessage, Alarms[alarmsBacklog]->buf[i]) == false
      && (Panel->flags.isMaster == true || Alarms[alarmsBacklog]->buf[i] == alConFailAtAllPanel)
    ) {
      if(Alarms[alarmsBacklog]->buf[i] == alConFailAtAllPanel)
      {
        Alarms[alarmsActual]->count = 0;
      }
      Alarms[alarmsActual]->buf[Alarms[alarmsActual]->count++] = Alarms[alarmsBacklog]->buf[i];
    }
  }

  for(i = 0; i < Alarms[Panel->flags.isMaster == true ? alarmsBacklog : alarmsActual]->count; i++) 
  {
    if(Panel->flags.isMaster == true ? isMasked(alarmsMaskIndicator, Alarms[alarmsBacklog]->buf[i]) == false : true) 
    {
      Panel->flags.noneCrash = false;
      return;
    }
  }
  Panel->flags.noneCrash = true;
  Panel->flags.cvitCrash = false;
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

      if (NumberCrash >= startAlarmsCon && NumberCrash < endAlarmsCon)
        addEvent(NumberCrash + (endAlarmsCon - startAlarmsCon));

      if ((GetPSBStatus(702) == false) 
        && (NumberCrash >= startAlarmsConGate && NumberCrash < endAlarmsConGate)
      ) {
        addEvent(NumberCrash + (endAlarmsConGate - startAlarmsConGate));
      }
		}
	}
}

void initAlarms(void)
{
  size_t i;
  
  Alarms[alarmsActual]  = (Alarms_t *)&PSW[FIRST_RR_ALARMS + 0];
  Alarms[alarmsBacklog] = (Alarms_t *)&PSW[FIRST_RR_ALARMS + 200];
  
  Alarms[alarmsSHOT]    = (Alarms_t *)&PSW[FIRST_RR_ALARMS_GATE + 0];
  Alarms[alarmsSHSN]    = (Alarms_t *)&PSW[FIRST_RR_ALARMS_GATE + 200];
  Alarms[alarmsSHSND]   = (Alarms_t *)&PSW[FIRST_RR_ALARMS_GATE + 400];

  setMask(alarmsMaskMessage, alPowerOn1, true);
  setMask(alarmsMaskMessage, alPowerOn2, true);
  setMask(alarmsMaskMessage, alPowerOn3, true);
  setMask(alarmsMaskMessage, alPowerOn4, true);
  setMask(alarmsMaskIndicator, alPowerOn1, true);
  setMask(alarmsMaskIndicator, alPowerOn2, true);
  setMask(alarmsMaskIndicator, alPowerOn3, true);
  setMask(alarmsMaskIndicator, alPowerOn4, true);
}

void finitAlarms(void)
{
  setMask(alarmsMaskMessage, alConFailAtAllPanel, false);
  setMask(alarmsMaskIndicator, alConFailAtAllPanel, false);
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


Alarm_t convertionNumberAlarm(Shield_t numberShield, uint16_t numberAlarm)
{

  switch (numberShield)
  {
    case shieldShot:
      switch (numberAlarm)
      {
        // включение панели + доступы
        case 1:  return alShotCon_1;   //  35
        case 2:  return alShotCon_2;   //  36
        case 3:  return alShotCon_3;   //  37
        // обрыв связи
        case 11: return alShotCon_4;   //  38
        case 12: return alShotCon_5;   //  39
        case 25: return alShotCon_6;   //  40
        case 28: return alShotCon_7;   //  41
        case 40: return alShotCon_8;   //  42
        case 36: return alShotCon_9;   //  43
        // связь восстановлена
        case 53: return alShotCon_10;   //  44
        case 54: return alShotCon_11;   //  45
        case 67: return alShotCon_12;   //  46
        case 70: return alShotCon_13;   //  47
        case 82: return alShotCon_14;   //  48
        case 78: return alShotCon_15;   //  49
        case 200: return alShotB118_1;  //  50
        case 201: return alShotB118_2;  //  51

        default:
          // AnP
          if (numberAlarm >= 214 && numberAlarm <= 229)  // 52...67
            return (numberAlarm - 162);
          // DP
          if (numberAlarm >= 262 && numberAlarm <= 301)  // 68...107
            return (numberAlarm - 194);
          // BKIf
          if (numberAlarm >= 614 && numberAlarm <= 627)  // 108...121
            return (numberAlarm - 506);
        } 
    break;
    case shieldShsn:
      switch (numberAlarm)
      {
        // включение панели + доступы
        case 1:  return alShsnCon_1;   //  122
        case 2:  return alShsnCon_2;   //  123
        case 3:  return alShsnCon_3;   //  124
        // обрыв связи
        case 28: return alShsnCon_4;   //  125
        // связь восстановлена
        case 70: return alShsnCon_5;   //  126

        default:
          // DP
          if (numberAlarm >= 262 && numberAlarm <= 281)   // 127...146
            return (numberAlarm - 135);
      } 
    break;
    case shieldShsnD:
      switch (numberAlarm)
      {
        // включение панели + доступы
        case 1:  return alShsnDCon_1;   //  147
        case 2:  return alShsnDCon_2;   //  148
        case 3:  return alShsnDCon_3;   //  149
        // обрыв связи
        case 28: return alShsnDCon_4;   //  150
        // связь восстановлена
        case 70: return alShsnDCon_5;   //  151

        default:
          // DP
          if (numberAlarm >= 262 && numberAlarm <= 281)   //  152...171
            return (numberAlarm - 110);
      } 
    break;
  }
  return alNone;
}


bool_t findAlarms(Alarms_t * pAlarms, Alarm_t number)
{
  size_t i;
  for(i = 0; i < COUNT_ALARMS && i < pAlarms->count; i++)
    if(pAlarms->buf[i] == number)
      return true;
  
  return false;
}
// local functions --------------------------------------------------------------------------------

uint16_t getMaskValue(AlarmsMask_t typeMask, Alarm_t numberAlarm)
{
  cell_t c; c.type = memPFW; c.number = FIRST_RR_CONFCRASH + NumberOFCrashes*typeMask + numberAlarm/16;
  return read(c).value;
}

