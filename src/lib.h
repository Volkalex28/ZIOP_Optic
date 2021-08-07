/**
 * @file lib.h
 * @author Volkalex28 (anan20000228@gmail.com)
 * @brief Служебный блок
 * Необходим для использования функций из библиотеки ОВЕН.
 * В процессе компиляции не учавствует, так-как отключается дефайном, который
 * описан к Глобальном макросе Конфигуратора
 */

#ifndef __LIB_H__
#define __LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _LIB_H
#define _LIB_H
// Include
#include <locale.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long       DWORD;
typedef unsigned char       BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned int        UINT;

typedef short int           Int16;
typedef int                 Int32;

typedef unsigned short      Uint16;
typedef unsigned int        Uint32;

enum {FALSE = 0, TRUE = 1};

// 
enum ECom
{
  HMI_LOCAL_MCH = -1,
  DOWNLOAD,
  PLC,
  EXPAND,
  REVERSE,
  NET_0,
  NET_1,
  NET_2,
  NET_3,
  NET_4,
  NET_5,
  NET_6,
  NET_7,
  NET_8,
  NET_9,
  EComMax
};

enum EInnerReg
{
  TYPE_PSB = 0,
  TYPE_PSW,
  TYPE_PFW,
  TYPE_PRW,
  TYPE_PHW,
  TYPE_PUW,
  TYPE_PCW,
  EInnerRegMax
};

enum EDataType
{
  TYPE_NONE,
  TYPE_BIT,
  TYPE_BYTE,
  TYPE_WORD,
  TYPE_DWORD,
  TYPE_REGS,
  TYPE_BYTE_3
};

// Method
/*******************************************************************************
 * Macro Function define
 ******************************************************************************/
typedef BOOL (*MacroFRead)(int comID, int staID, int objType, int add1, int add2, int dataType, void* pValue);
typedef BOOL (*MacroFReads)(int comID, int staID, int objType, int add1, int regs, void* pRegs);
typedef BOOL (*MacroFWrite)(int comID, int staID, int objType, int add1, int add2, int dataType, DWORD dwValue);
typedef BOOL (*MacroFWrites)(int comID, int staID, int objType, int add1, int regs, void* pRegs);

#ifndef _MODEL_TH
typedef BOOL (*MacroFXRead)(int comID, int netID, int staID, int objType, int add1, int add2, int dataType, void* pValue);
typedef BOOL (*MacroFXReads)(int comID, int netID, int staID, int objType, int add1, int regs, void* pRegs);
typedef BOOL (*MacroFXWrite)(int comID, int netID, int staID, int objType, int add1, int add2, int dataType, DWORD dwValue);
typedef BOOL (*MacroFXWrites)(int comID, int netID, int staID, int objType, int add1, int regs, void* pRegs);
#endif

typedef void (*MacroFComOpen)( BYTE ComNO );
typedef void (*MacroFComClose)( BYTE ComNO );
typedef BOOL (*MacroFComSend)( BYTE ComNO, BYTE *SndBuf, WORD Len );
typedef WORD (*MacroFComRcv)( BYTE ComNO, BYTE *RcvBuf, WORD Len, WORD TimeOut, BYTE TimeOutBytes);

typedef void* (*MacroFHMI_malloc)( UINT size );
typedef void (*MacroFHMI_free)( void *pointer );
typedef void (*MacroFOSSchedLock)( void );
typedef void (*MacroFOSSchedUnlock)( void );
typedef void (*MacroFDelayMs)( UINT num );

typedef WORD (*MacroFScreenJump)( WORD ScreenNo );
typedef void (*MacroFOpenWindow)( WORD winNo, WORD Window_X, WORD Window_Y );
typedef void (*MacroFCloseWindow)( WORD winNo );
typedef void (*MacroFBeepOnce)( void );
typedef void (*MacroFLightScreen)( void );
typedef UINT (*MacroFCRC)( BYTE *pHead, UINT Len );
typedef UINT (*MacroFGeneralCRC_16)( BYTE *Input, UINT Len, UINT poly, UINT good_crc );
typedef float (*MacroFBendCalYDepth)(float fAngle, float fThick, float fHatch, DWORD nStrenth, DWORD nEMode);
typedef DWORD (*MacroFBendCalForce)( float fThick, float fWidth, float fDHatch );

typedef BOOL (*MacroFDCMapClear)( DWORD dwDCMapID );
typedef BOOL (*MacroFDCMapSetBackColor)( DWORD dwDCMapID, DWORD BackColor );
typedef BOOL (*MacroFDCMapDrawLine)( DWORD dwDCMapID, int x, int y, int Width, int Height, int linewidth, DWORD color );
typedef BOOL (*MacroFDCMapDrawRect)( DWORD dwDCMapID, int x, int y, int Width, int Height, int linewidth, DWORD color, BOOL FillRect, DWORD FillColor );
typedef BOOL (*MacroFDCMapDrawCircle)( DWORD dwDCMapID, int x, int y, int Radius, int linewidth, DWORD color, BOOL FillRect, DWORD FillColor );
typedef BOOL (*MacroFDCMapDrawEllipse)( DWORD dwDCMapID, int x, int y, int X_Axis_Len, int Y_Axis_Len, int linewidth, DWORD color, BOOL FillRect, DWORD FillColor );
typedef BOOL (*MacroFDCMapDrawCircleArc)( DWORD dwDCMapID, int x, int y, int Radius, int linewidth, DWORD color, DWORD StartAngle, DWORD EndAngle );
typedef BOOL (*MacroFDCMapDrawEllipseArc)( DWORD dwDCMapID, int x, int y, int X_Axis_Len, int Y_Axis_Len, int linewidth, DWORD color, DWORD StartAngle, DWORD EndAngle );

#ifndef _MODEL_TH
typedef FILE * (*Macrofopen)(const char * path,const char * mode);
typedef int (*Macrofclose)(FILE *stream);
typedef int (*Macrofseek)(FILE* fp, long offset, int whence);
typedef void (*Macrorewind)(FILE* fp);
typedef int (*Macroremove)(const char* filename);
typedef size_t (*Macrofread)(void* ptr, size_t size, size_t n, FILE* fp);
typedef size_t (*Macrofwrite)(const void* ptr, size_t size, size_t n, FILE* fp);
typedef long (*Macroftell)(FILE *stream);
#endif

// struct
#pragma arm section zidata = "MacroTable"
extern struct MACRO_CALL_BACK
{
  WORD*				pPSW;
  BYTE*				pPSB;
  MacroFRead 			pRead;
  MacroFReads 		pReads;
  MacroFWrite 		pWrite;
  MacroFWrites 		pWrites;

  MacroFComOpen		pComOpen;
  MacroFComClose		pComClose;
  MacroFComSend		pComSend;
  MacroFComRcv		pComRcv;
  MacroFHMI_malloc	pHMI_malloc;
  MacroFHMI_free		pHMI_free;
  MacroFOSSchedLock	pOSSchedLock;
  MacroFOSSchedUnlock	pOSSchedUnlock;
  MacroFDelayMs		pDelayMs;

  MacroFScreenJump	pScreenJump;
  MacroFOpenWindow	pOpenWindow;
  MacroFCloseWindow	pCloseWindow;
  MacroFBeepOnce		pBeepOnce;
  MacroFLightScreen	pLightScreen;
  MacroFCRC			pCRC;
  MacroFGeneralCRC_16	pGeneralCRC_16;
  MacroFBendCalYDepth pBendCalYDepth;
  MacroFBendCalForce  pBendCalForce;

  MacroFDCMapClear	pDCMapClear;
  MacroFDCMapSetBackColor pDCMapSetBackColor;
  MacroFDCMapDrawLine		pDCMapDrawLine;
  MacroFDCMapDrawRect		pDCMapDrawRect;
  MacroFDCMapDrawCircle	pDCMapDrawCircle;
  MacroFDCMapDrawEllipse pDCMapDrawEllipse;
  MacroFDCMapDrawCircleArc pDCMapDrawCircleArc;
  MacroFDCMapDrawEllipseArc	pDCMapDrawEllipseArc;

  #ifndef _MODEL_TH
  MacroFXRead 		pXRead;
  MacroFXReads 		pXReads;
  MacroFXWrite 		pXWrite;
  MacroFXWrites 		pXWrites;

  Macrofopen			pfopen;
  Macrofclose			pfclose;
  Macrofseek			pfseek;
  Macrorewind			prewind;
  Macroremove			premove;
  Macrofread			pfread;
  Macrofwrite			pfwrite;
  Macroftell			pftell;
  #endif
} Macro;
#pragma arm section zidata

// define
#define	PSW					(Macro.pPSW)
#define PSB					(Macro.pPSB)
#define Read				(Macro.pRead)
#define Reads				(Macro.pReads)
#define Write				(Macro.pWrite)
#define Writes				(Macro.pWrites)
#define XNetRead			(Macro.pXRead)
#define XNetReads			(Macro.pXReads)
#define XNetWrite			(Macro.pXWrite)
#define XNetWrites			(Macro.pXWrites)
#define Send				(Macro.pComSend)
#define Receive				(Macro.pComRcv)
#define Enter				(Macro.pComOpen)
#define Leave				(Macro.pComClose)
#define Malloc				(Macro.pHMI_malloc)
#define Free				(Macro.pHMI_free)
#define Lock				(Macro.pOSSchedLock)
#define	Unlock				(Macro.pOSSchedUnlock)
#define Delay				(Macro.pDelayMs)

#define ScreenJump			(Macro.pScreenJump)
#define OpenWindow			(Macro.pOpenWindow)
#define CloseWindow			(Macro.pCloseWindow)
#define Beep				(Macro.pBeepOnce)
#define LightScreen			(Macro.pLightScreen)
#define WakeupScreen		(Macro.pLightScreen)
#define Crc					(Macro.pCRC)
#define Crc16				(Macro.pGeneralCRC_16)
#define BendCalYDepth		(Macro.pBendCalYDepth)
#define BendCalForce		(Macro.pBendCalForce)

#define DCMapClear			(Macro.pDCMapClear)
#define DCMapSetBackColor	(Macro.pDCMapSetBackColor)
#define DCMapDrawLine		(Macro.pDCMapDrawLine)
#define DCMapDrawRect		(Macro.pDCMapDrawRect)
#define DCMapDrawCircle		(Macro.pDCMapDrawCircle)
#define DCMapDrawEllipse			(Macro.pDCMapDrawEllipse)
#define DCMapDrawCircleArc			(Macro.pDCMapDrawCircleArc)
#define DCMapDrawEllipseArc			(Macro.pDCMapDrawEllipseArc)

#define fopen				(Macro.pfopen)
#define fclose				(Macro.pfclose)
#define fseek				(Macro.pfseek)
#define frewind			(Macro.prewind)
#define fremove			(Macro.premove)
#define fread				(Macro.pfread)
#define fwrite				(Macro.pfwrite)
#define ftell				(Macro.pftell)

// math
#define Max(a,b)            (((a) > (b)) ? (a) : (b))
#define Min(a,b)            (((a) < (b)) ? (a) : (b))

#define MAKEWORD(byl, byh)    ((WORD)(((BYTE)(byl)) | ((WORD)((BYTE)(byh))) << 8))
#define MAKEDWORD(wl, wh)      ((DWORD)(((WORD)(wl)) | ((DWORD)((WORD)(wh))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

// data convert
#define Word_2_Int16(w)		((short)(w))
#define Word_2_Int32(w)		((int)(short)(w))
#define DWord_2_Int32(dw)	((int)(dw))
#define DWord_2_Float(dw)	(*(float*)&(dw))

#define Int16_2_Word(s)		((WORD)(s))
#define Int32_2_Word(s)		((WORD)(short)(s))
#define Int32_2_DWord(i)	((DWORD)(i))
#define Float_2_DWord(f)	(*(DWORD*)&(f))

// PSB
#define 	BITS_PER_BYTE 8
#define		GetPSBStatus( PSB_No )		( ( PSB[ (PSB_No) / BITS_PER_BYTE ] & ( 1 << ( (PSB_No) % BITS_PER_BYTE ) ) ) != 0 )
#define		SetPSB( PSB_No )			PSB[ (PSB_No) / BITS_PER_BYTE ] |= ( 1 << ( (PSB_No) % BITS_PER_BYTE ) )
#define		ResetPSB( PSB_No )			PSB[ (PSB_No) / BITS_PER_BYTE ] &= ( ~ ( 1 << ( (PSB_No) % BITS_PER_BYTE  ) ) )

#endif

#ifdef __cplusplus
}
#endif

enum MODBUS_RTU_BIT {
  MODBUS_RTU_BIT_0X = 0,
  MODBUS_RTU_BIT_1X = 1,
  MODBUS_RTU_BIT_4X = 3
};
enum MODBUS_RTU_REG {
  MODBUS_RTU_REG_4X = 3,
  MODBUS_RTU_REG_3X = 2
};
enum MODBUS_RTU_REGS {
  MODBUS_RTU_REGS_4X = 3,
  MODBUS_RTU_REGS_3X = 2
};

typedef enum EBool {true, false} bool;

#endif // __LIB_H__
