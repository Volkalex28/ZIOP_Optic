/**
 * @file shsn.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREEN_SHSN_H__
#define __SCREEN_SHSN_H__

#include "../lib.h"

typedef struct ScreenShsn_s 
{
  SCREEN_HEADER(SettScreenShsn_s,
    struct VisibleSettScreenShsn_s 
    {
      uint16_t _0       : 1;    // 0
      uint16_t UmaxUst  : 1;    // 1
      uint16_t UminUst  : 1;    // 2
    } Visible;                  // 50
    struct EventSettScreenShsn_s 
    {
      uint16_t Vv1_220_inW 	: 1;
      uint16_t Vv2_220_inW  : 1;
      uint16_t Vv1_36_inW 	: 1;
      uint16_t Vv2_36_inW   : 1;
      uint16_t Vv1_220_offA	: 1;
      uint16_t Vv2_220_offA	: 1;
      uint16_t Vv1_36_offA 	: 1;
      uint16_t Vv2_36_offA 	: 1;
      uint16_t Choose 		: 1;
    } Event;                    // 51
      char TitleVv1_220[20];  //552..561
      char TitleVv2_220[20];  //562..571
      char TitleVv1_36[20];   //572..581
      char TitleVv2_36[20];   //582..591
      char TitleChoose[6];    //592..594
  );
} ScreenShsn_t;

void screenShsn(void);

#endif // __SHSN_H__

