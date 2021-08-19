/**
 * @file manager.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "manager.h"
#include "panel.h"
#include "pfw.h"

#include "../alarms.h"
#include "../devices/devices_mem.h"
#include "../screens/screen.h"

Alarms_t      * Alarms;

com_t get_com(MemTypes_t type);

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
      );
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
    if(com != EComMax)
      cell.status = Write(
        com, 
        cell.adress, 
        com < NET_0 ? MODBUS_RTU_REG_4X : MODBUS_TCP_REG_4X, 
        cell.number, 
        0, 
        TYPE_WORD, 
        cell.value
      );
    else
      cell.status = memStatusIncorrectData;
      
    break;
  }
  get_ptr(&cell);

  return cell;
}

cell_t reads(cell_t cell, uint16_t count, uint16_t * pvalues) 
{
  int i;
  com_t com = get_com(cell.type);

  switch (cell.type)
  {
  case memPFW:
    cell.status = Reads(HMI_LOCAL_MCH, 0, TYPE_PFW, cell.number, count, pvalues);
    break;
  case memPSW:
    cell.status = Reads(HMI_LOCAL_MCH, 0, TYPE_PSW, cell.number, count, pvalues);
    break;
  case memPSB:
    cell.status = ((cell.number + count) < 1024) ? memStatusOK : memStatusFAIL;
    if(cell.status != memStatusOK) break;
    for(i = 0; i < count; i++)
      pvalues[i] = GetPSBStatus(cell.number + i);
    break;

  default:
    if(com != EComMax)
      cell.status = Reads(
        com, 
        cell.adress, 
        com < NET_0 ? MODBUS_RTU_REGS_4X : MODBUS_TCP_REGS_4X, 
        cell.number, 
        count, 
        pvalues
      );
    else
      cell.status = memStatusIncorrectData;
    break;
  }
  get_ptr(&cell);

  return cell;
}

cell_t writes(cell_t cell, uint16_t count, uint16_t * pvalues) 
{
  int i;
  com_t com = get_com(cell.type);

  switch (cell.type)
  {
  case memPFW:
    cell.status = Writes(HMI_LOCAL_MCH, 0, TYPE_PFW, cell.number, count, pvalues);
    break;
  case memPSW:
    cell.status = Writes(HMI_LOCAL_MCH, 0, TYPE_PSW, cell.number, count, pvalues);
    break;
  case memPSB:
    cell.status = ((cell.number + count) < 1024) ? memStatusOK : memStatusFAIL;
    if(cell.status != memStatusOK) break;
    for(i = 0; i < count; i++)
      if(pvalues[i] != 0) SetPSB(cell.number + i); else ResetPSB(cell.number + i);
    break;

  default:
    if(com != EComMax)
      cell.status = Writes(
        com, 
        cell.adress, 
        com < NET_0 ? MODBUS_RTU_REGS_4X : MODBUS_TCP_REGS_4X, 
        cell.number, 
        count, 
        pvalues
      );
    else
      cell.status = memStatusIncorrectData;
    break;
  }
  get_ptr(&cell);

  return cell;
}

void mem_init() 
{
  Panel   = (Panel_t *)         &PSW[FIRST_RR_PANEL];
  PFW     = (PFW_t *)           &PSW[FIRST_RR_FOR_EEP];
  Screens = (Screens_t *)       &PSW[FIRST_RR_SCREEN];
  Alarms  = (Alarms_t *)        &PSW[FIRST_RR_ALARMS];
  dMem    = (devices_mem_t * )  &PSW[FIRST_RR_DEV_MEM];
  // T1      = (Trans_t * )        &PSW[FIRST_RR_TRANS];
  // T2      = (Trans_t * )        &PSW[FIRST_RR_TRANS+2];
  // T3      = (Trans_t * )        &PSW[FIRST_RR_TRANS+4];
  // T4      = (Trans_t * )        &PSW[FIRST_RR_TRANS+6];
  dMemPFW = &PFW->dMemPFW;

  getPFW();
}

com_t get_com(MemTypes_t type)
{
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

void get_ptr(cell_t * cell) 
{
  switch (cell->type)
  {
  case memPFW:
    if(cell->number < FIRST_RR_EEP || cell->number > (FIRST_RR_EEP + COUNT_RR_EEP))
      cell->ptr = NULL;
    else
      cell->ptr = &PSW[FIRST_RR_FOR_EEP + (cell->number - FIRST_RR_EEP)];
    break;

  case memPSW:
    if(cell->number >= 4096)
      cell->ptr = NULL;
    else
      cell->ptr = &PSW[cell->number];
    break;
  
  default:
    cell->ptr = NULL;
    break;
  }
}

void clearRRScreens()
{
  int i;

  if(PSW[1] == 11)
    return;

  for(i = 0; i < 50; i++)
  {
    PSW[FIRST_RR_SCREEN + i] = 0;
    if(Panel->oldScreen != PSW[1])
      PSW[FIRST_RR_SCREEN + 50 + i] = 0;
    CAST_TO_PU16(PSW[FIRST_RR_SCREEN + 100 + i*2]) = NULL;
  }
}

void fillRRScreens()
{
  int i;

  if(PSW[1] == 11)
    return;

  for(i = 0; i < 50; i++)
  {
    PSW[FIRST_RR_SCREEN + i] = CAST_TO_PU16(PSW[FIRST_RR_SCREEN + 100 + i*2]) != NULL 
                                ? *CAST_TO_PU16(PSW[FIRST_RR_SCREEN + 100 + i*2]) 
                                : 0;
  }
}
