/**
 * @file manager.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

/**
 * @defgroup Data Данные и память
 * 
 * Содержит функции для выделения инициализации памяти панели, а также занмается
 * управлением внутренними и внешними данными
 */

#include <string.h>

#include "manager.h"

#include "pfw.h"

// local functions --------------------------------------------------------------------------------
/**
 * @defgroup Manager_Functions Локальные функции
 * @ingroup Manager
 * @{
 */

/**
 * @brief Функция получения порта для для обмена с внешними устройствами
 * 
 * @param type Тип памяти для обмена
 * @return Порт для обмена данными
 * @retval com_t Если тип памяти поддерживает обмен с внешними устройствами
 * @retval EComMax Если тип памяти не поддерживает обмен с внешними устройствами
 * 
 */
com_t get_com(MemTypes_t type);

///@}

cell_t read(cell_t cell) 
{  
  com_t com = get_com(cell.type);

  switch (cell.type)
  {
  case memPFW: 
    cell.status = Read(HMI_LOCAL_MCH, 0, TYPE_PFW, cell.number, 0, TYPE_WORD, &cell.value); 
    break;
  case memPSW: 
    cell.status = Read(HMI_LOCAL_MCH, 0, TYPE_PSW, cell.number, 0, TYPE_WORD, &cell.value);
    break;
  case memPSB: 
    cell.status = Read(HMI_LOCAL_MCH, 0, TYPE_PSB, cell.number, 0, TYPE_BIT, &cell.value);
    break;  

  default:
    if(com != EComMax)
      cell.status = Read(com, 
        cell.adress, 
        com < NET_0 ? MODBUS_RTU_REG_4X : MODBUS_TCP_REG_4X, 
        cell.number, 
        0, 
        TYPE_WORD, 
        &cell.value
      ) == 1 ? memStatusOK : memStatusFAIL;
    else
      cell.status = memStatusIncorrectData;
    break;
  }
  get_ptr(&cell);

  return cell;
}

cell_t write(cell_t cell) 
{
  com_t com = get_com(cell.type);

  switch (cell.type)
  {
  case memPFW: 
    cell.status = Write(HMI_LOCAL_MCH, 0, TYPE_PFW, cell.number, 0, TYPE_WORD, cell.value); 
    break;
  case memPSW: 
    cell.status = Write(HMI_LOCAL_MCH, 0, TYPE_PSW, cell.number, 0, TYPE_WORD, cell.value);
    break;
  case memPSB: 
    cell.status = Write(HMI_LOCAL_MCH, 0, TYPE_PSB, cell.number, 0, TYPE_BIT, cell.value);
    break;  

  default:
    if(com != EComMax && !GetPSBStatus(700 + cell.type - net0))
      cell.status = Write(
        com, 
        cell.adress, 
        com < NET_0 ? MODBUS_RTU_REG_4X : MODBUS_TCP_REG_4X, 
        cell.number, 
        0, 
        TYPE_WORD, 
        cell.value
      ) == 1 ? memStatusOK : memStatusFAIL;
    else
      cell.status = memStatusIncorrectData;
      
    break;
  }
  get_ptr(&cell);

  return cell;
}

cell_t reads(cell_t cell, uint16_t count) 
{
  size_t i;
  com_t com = get_com(cell.type);

  switch (cell.type)
  {
  case memPFW:
    cell.status = Reads(HMI_LOCAL_MCH, 0, TYPE_PFW, cell.number, count, cell.ptr);
    break;
  case memPSW:
    cell.status = Reads(HMI_LOCAL_MCH, 0, TYPE_PSW, cell.number, count, cell.ptr);
    break;
  case memPSB:
    cell.status = ((cell.number + count) < 1024) ? memStatusOK : memStatusFAIL;
    if(cell.status != memStatusOK) break;
    for(i = 0; i < count; i++)
      cell.ptr[i] = GetPSBStatus(cell.number + i);
    break;

  default:
    if(com != EComMax)
    {
      cell.status = memStatusOK;
      for(i = 0; i <= (count-1)/125 && cell.status == memStatusOK; i++)
      {
        cell.status = Reads(com, cell.adress, 
          com < NET_0 ? MODBUS_RTU_REGS_4X : MODBUS_TCP_REGS_4X, 
          cell.number + 125*i, 
          count/125 - i > 0 ? 125 : count%125, 
          cell.ptr + 125*i
        ) == 1 ? memStatusOK : memStatusFAIL;
      }

      if(cell.status == memStatusFAIL && cell.ptr != NULL)
      {
        memset(cell.ptr, 0, count * 2);
      }
    }
    else
      cell.status = memStatusIncorrectData;
    break;
  }
  get_ptr(&cell);

  return cell;
}

cell_t writes(cell_t cell, uint16_t count) 
{
  size_t i;
  com_t com = get_com(cell.type);

  switch (cell.type)
  {
  case memPFW:
    cell.status = Writes(HMI_LOCAL_MCH, 0, TYPE_PFW, cell.number, count, cell.ptr);
    break;
  case memPSW:
    cell.status = Writes(HMI_LOCAL_MCH, 0, TYPE_PSW, cell.number, count, cell.ptr);
    break;
  case memPSB:
    cell.status = ((cell.number + count) < 1024) ? memStatusOK : memStatusFAIL;
    if(cell.status != memStatusOK) break;
    for(i = 0; i < count; i++)
      if(cell.ptr[i] != 0) SetPSB(cell.number + i); else ResetPSB(cell.number + i);
    break;

  default:
    if(com != EComMax && !GetPSBStatus(700 + cell.type - net0))
    {
      cell.status = memStatusOK;
      for(i = 0; i <= (count-1)/125 && cell.status == memStatusOK; i++)
      {
        cell.status = Writes(
          com, 
          cell.adress, 
          com < NET_0 ? MODBUS_RTU_REGS_4X : MODBUS_TCP_REGS_4X, 
          cell.number + 125*i, 
          count/125 - i > 0 ? 100 : count%125, 
          cell.ptr + 125*i
        ) == 1 ? memStatusOK : memStatusFAIL;
      }
    }
    else
      cell.status = memStatusIncorrectData;
    break;
  }
  get_ptr(&cell);

  return cell;
}

void get_ptr(cell_t * cell) 
{
  switch (cell->type)
  {
  case memPFW:
    if(!(cell->number < FIRST_RR_EEP || cell->number > (FIRST_RR_EEP + COUNT_RR_EEP)))
      cell->ptr = &PSW[FIRST_RR_FOR_EEP + (cell->number - FIRST_RR_EEP)];
    break;

  case memPSW:
    if(cell->number < 4096)
      cell->ptr = &PSW[cell->number];
    break;
  
  // default:
  //   cell->ptr = NULL;
  //   break;
  }
}

void initMem(void) 
{ }

void selectCircle(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down) 
{
  if(Up)   (*Select)++;
  if(Down) (*Select)--;

  if(*Select < Min) (*Select) = Max; 
  if(*Select > Max) (*Select) = Min;
}

void selectNormal(int16_t * Select, int16_t Max, int16_t Min, bool_t Up, bool_t Down) 
{
  if(Up)   (*Select)++;
  if(Down) (*Select)--;

  if(*Select < Min) (*Select) = Min; 
  if(*Select > Max) (*Select) = Max;
}

void selectNormalBlock(int16_t * Select, uint16_t Offset, int16_t Max, int16_t Min, bool_t Up, bool_t Down) 
{
  if(Up)   (*Select) += Offset;
  if(Down) (*Select) -= Offset;

  if(*Select < Min) (*Select) = Min; 
  if(*Select > Max) (*Select) = Max;
}

// local functions --------------------------------------------------------------------------------

com_t get_com(MemTypes_t type)
{
  if(type >= net0 && type <= net5)
  {
    if((getEnable(type - net0) == false))
      return EComMax;
  }

  switch (type)
  {
  case portDownload:  return DOWNLOAD;
  case portPLC:       return PLC;

  case net0: return NET_0; break;
  case net1: return NET_1; break;
  case net2: return NET_2; break;
  case net3: return NET_3; break;
  case net4: return NET_4; break;
  case net5: return NET_5; break;
  
  default: return EComMax;
  }
}
