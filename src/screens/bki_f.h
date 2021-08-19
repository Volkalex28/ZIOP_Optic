/**
 * @file bki_f.h
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#ifndef __SCREEN_BKIF_H__
#define __SCREEN_BKIF_H__

#include "../lib.h"

typedef struct ScreenBkif_s 
{
  SCREEN_HEADER(SettScreenBkif_s,
      short Title[3];			// 50 - 52
      struct {
        short Title[2];	// 0 - 1
        short dimention;// 2
      } title[3];				// 53 - 61
      short Select;			// 62
      struct EventSettScreenBkif_s{
        unsigned short SelectUp		: 1; // 0
        unsigned short SelectDown	: 1; // 1
        unsigned short FiderUp		: 1; // 2
        unsigned short FiderDown	: 1; // 3
      } Event;				// 63
      struct VisibleSettScreenBkif_s{
        unsigned short Select		: 1; // 0
        unsigned short SelectFider	: 1; // 1
        unsigned short _1			: 1; // 2
        unsigned short _2			: 1; // 3
        unsigned short _3			: 1; // 4
      } Visible;				// 64
      short SelectFider;		// 65
  );

  short* _1;
  short* _2;
  short* _3;
  short* _4;
  short* _5;
  short* _6;
  short* Nfider;
  short* KalibrK;
  short* LevelR;
  short* En_Sekcion;
} ScreenBkif_t;

void screenBkif(void);

#endif // __BKIF_H__




  struct BKIf_s {
    short ZnachReg[50];

    struct SettBKIf {
      short Title[3];			// 50 - 52
      struct {
        short Title[2];	// 0 - 1
        short dimention;// 2
      } title[3];				// 53 - 61
      short Select;			// 62
      struct {
        unsigned short SelectUp		: 1; // 0
        unsigned short SelectDown	: 1; // 1
        unsigned short FiderUp		: 1; // 2
        unsigned short FiderDown	: 1; // 3
      } Event;				// 63
      struct {
        unsigned short Select		: 1; // 0
        unsigned short SelectFider	: 1; // 1
        unsigned short _1			: 1; // 2
        unsigned short _2			: 1; // 3
        unsigned short _3			: 1; // 4
      } Visible;				// 64
      short SelectFider;		// 65
    } Setting;
    char Empty[100-sizeof(struct SettBKIf)];

    short* _1;
    short* _2;
    short* _3;
    short* _4;
    short* _5;
    short* _6;
    short* Nfider;
    short* KalibrK;
    short* LevelR;
    short* En_Sekcion;
  } BKIf;

