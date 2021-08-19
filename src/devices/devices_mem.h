/**
 * @file devices_mem.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __DEVICES_MEM_H__
#define __DEVICES_MEM_H__

#include "dDP.h"

typedef struct devices_mem_s
{
  dDPmem_t DP[N_DP]; 

  struct Transformer_s
  {
    struct OnOff_s
    {
      int16_t Switch_Q19    :1;
      int16_t PullOut_Q19   :1;
      int16_t Ground_Q19    :1;
      int16_t PullOut_QST1  :1;
      int16_t Ground_QSGT1  :1;
      int16_t Switch_Q1     :1;
      int16_t PullOut_Q1    :1;
      int16_t Ground_QSG1   :1;
      int16_t PullOut_Q13   :1;
      int16_t Ground_Q13    :1;
      int16_t ArcProtection :1;
      int16_t NoCurrent     :1;
      int16_t WarmWarning   :1;
      int16_t WarmCrash     :1;
    } OnOff;                            
    struct undefined_s 
    {
      int16_t Switch_Q19    :1;
      int16_t PullOut_Q19   :1;
      int16_t Ground_Q19    :1;
      int16_t PullOut_QST1  :1;
      int16_t Ground_QSGT1  :1;
      int16_t Switch_Q1     :1;
      int16_t PullOut_Q1    :1;
      int16_t Ground_QSG1   :1;
      int16_t PullOut_Q13   :1;
      int16_t Ground_Q13    :1;
      int16_t ArcProtection :1;
      int16_t Switch_Q19_CO    :1;
      int16_t PullOut_Q19_CO   :1;
      int16_t Ground_Q19_CO    :1;
    } Undefined;
    uint16_t oldScreen;             
  } Trans_t;

} devices_mem_t;

typedef struct devices_pfw_s
{
  dmain_t DP[N_DP]; 
} devices_pfw_t;

extern devices_mem_t * dMem;
extern devices_pfw_t * dMemPFW;

#endif // __DEVICES_MEM_H__