/**
 * @file crashset.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __CRASHSET_H__
#define __CRASHSET_H__

#include "../lib.h"
#include "../screens/screen.h"

#define NumberOFCrashes   (((alAll % 16) != 0) ? alAll / 16 + 1 : alAll / 16)


uint16_t ReadState[NumberOFCrashes*3];   
// uint16_t ReadMCrash[NumberOFCrashes];  
// uint16_t ReadMEvent[NumberOFCrashes];  

// typedef struct PFWConfigCrash_s
// {
//   uint16_t ConfCrash_OnOff[NumberOFCrashes];
//   uint16_t ConfCrash_MaskaCrash[NumberOFCrashes];
//   uint16_t ConfCrash_MaskaEvent[NumberOFCrashes];
// } PFWConfigCrash_t;

// extern PFWConfigCrash_t * PFWConfigCrash;


void FillAndEditConfigFields (void);
void conversion(void);

#endif // __CRASHSET_H__

