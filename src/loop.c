/**
 * @file loop.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

/**
 * @defgroup Core Основные модули
 * 
 * Содержит такие модули как "Основной цикл работы", "Обмен", 
 * "Аварийные и системные сообщения" и т.д.
 */

#include "lib.h"

#include "alarms.h"
#include "init.h"

#include "mem/panel.h"
#include "mem/pfw.h"

#include "devices/dGate.h"

#include "screens/screen.h"

#define getBit(_RR_, _BIT_) ((PSW[_RR_] & (1 << (_BIT_))) ? true : false)
#define alExContr(_NC_, _COND_) if (_COND_) addCrash(_NC_); else deleteCrash(_NC_) 
#define controlBit(_N_, _BIT_, _EN_, _DIS_, _COND_)                                 \
  if(_COND_) {                                                                      \
    if(getBit((2502 + (_BIT_) / 16) + 4*(_N_), (_BIT_) % 16) == false && (_EN_)) {  \
      PSW[(1000 + (_BIT_) / 16) + 2*(_N_)] |= (1 << ((_BIT_) % 16));                \
      PSW[1100] |= (1 << (_N_ + 8*((int)(_BIT_) / 16)));                            \
    }                                                                               \
  } else {                                                                          \
    if(getBit((2502 + (_BIT_) / 16) + 4*(_N_), (_BIT_) % 16) == true && (_DIS_)) {  \
      PSW[(1000 + (_BIT_) / 16) + 2*(_N_)] &= ~(1 << ((_BIT_) % 16));               \
      PSW[1100] |= (1 << (_N_ + 8*((int)(_BIT_) / 16)));                            \
    }                                                                               \
  }

void diffConrtol(uint16_t inNum, uint16_t outNum, uint8_t fbit, uint8_t sbit, uint8_t ofbit, uint8_t osbit)
{
  if((PSW[inNum + fbit/16] & (1 << (fbit%16))) && !(PSW[inNum + sbit/16] & (1 << (sbit%16))))
  {
    PSW[outNum + ofbit/16] &= ~(1 << (ofbit%16));
    PSW[outNum + osbit/16] &= ~(1 << (osbit%16));
  }
  else if(!(PSW[inNum + fbit/16] & (1 << (fbit%16))) && (PSW[inNum + sbit/16] & (1 << (sbit%16))))
  {
    PSW[outNum + ofbit/16] |= (1 << (ofbit%16));
    PSW[outNum + osbit/16] &= ~(1 << (osbit%16));
  }
  else
  {
    PSW[outNum + ofbit/16] &= ~(1 << (ofbit%16));
    PSW[outNum + osbit/16] |= (1 << (osbit%16));
  }
}

void normConrtol(uint16_t inNum, uint16_t outNum, uint8_t bit, uint8_t obit, bool_t inversion)
{
  if((PSW[inNum + bit/16] & (1 << (bit%16))))
    PSW[outNum + obit/16] |= (1 << (obit%16));
  else
    PSW[outNum + obit/16] &= ~(1 << (obit%16));
  
  if(inversion)
    PSW[outNum + obit/16] ^= (1 << (obit%16));
}

void handlerLogic(void)
{
  //- 51 -----------------------------------------------------------------------------------------
  // inputs
  normConrtol(2500, 2533, 9, 0, false); // 2533.0 = Неисправность коммутатора оптического КО14 = K1.DI10
  normConrtol(2501, 2533, 3, 1, false); // 2533.1 = Неисправность коммутатора оптического КО15 = K1.DI20

  diffConrtol(2500, 2525, 0, 1, 0, 1);  // 2525.0 + 2525.1 = Выключатль Q1-9 = K1.DI1 + K1.DI2
  diffConrtol(2500, 2525, 2, 3, 3, 4);  // 2525.3 + 2525.4 = Выдв. элемент Q1-9 = K1.DI3 + K1.DI4
  diffConrtol(2500, 2525, 4, 5, 5, 6);  // 2525.5 + 2525.6 = Зазем. элемент Q1-9 = K1.DI5 + K1.DI6
  
  diffConrtol(2500, 2529, 10, 11, 0, 1); // 2529.0 + 2529.1 = Выключатль Q1-7 = K1.DI11 + K1.DI12
  diffConrtol(2500, 2529, 12, 13, 3, 4); // 2529.3 + 2529.4 = Выдв. элемент Q1-7 = K1.DI13 + K1.DI14
  diffConrtol(2500, 2529, 14, 15, 5, 6); // 2529.5 + 2529.6 = Зазем. элемент Q1-7 = K1.DI15 + K1.DI16
  
  // outputs
  normConrtol(2502, 2534, 0, 0, true);  // 2534.0 = Блокировка ВЭ выключателя Q1-9 = K1.DO1
  normConrtol(2502, 2534, 1, 1, true);  // 2534.1 = Блокировка ЗЭ Q1-9 = K1.DO2
  normConrtol(2502, 2534, 2, 2, true);  // 2534.2 = Отключение выключателя Q1-9 = K1.DO3

  normConrtol(2502, 2534, 4, 4, true);  // 2534.4 = Блокировка ВЭ выключателя Q1-7 = K1.DO5
  normConrtol(2502, 2534, 5, 5, true);  // 2534.5 = Блокировка ЗЭ Q1-7 = K1.DO6
  normConrtol(2502, 2534, 6, 6, true); // 2534.6 = Отключение выключателя Q1-7 = K1.DO7

  //- 52 -----------------------------------------------------------------------------------------
 // inputs
  normConrtol(2504, 2533, 9, 2, false); // 2533.2 = Неисправность коммутатора оптического КО14 = K2.DI10
  normConrtol(2505, 2533, 3, 3, false); // 2533.3 = Неисправность коммутатора оптического КО14 = K2.DI20

  diffConrtol(2504, 2527, 0, 1, 0, 1);  // 2527.0 + 2527.1 = Выключатль Q1-8 = K2.DI1 + K2.DI2
  diffConrtol(2504, 2527, 2, 3, 3, 4);  // 2527.3 + 2527.4 = Выдв. элемент Q1-8 = K2.DI3 + K2.DI4
  diffConrtol(2504, 2527, 4, 5, 5, 6);  // 2527.5 + 2527.6 = Зазем. элемент Q1-8 = K2.DI5 + K2.DI6
  
  diffConrtol(2504, 2531, 10, 11, 0, 1); // 2531.0 + 2531.1 = Выключатль Q1-10 = K2.DI11 + K2.DI12
  diffConrtol(2504, 2531, 12, 13, 3, 4); // 2531.3 + 2531.4 = Выдв. элемент Q1-10 = K2.DI13 + K2.DI14
  diffConrtol(2504, 2531, 14, 15, 5, 6); // 2531.5 + 2531.6 = Зазем. элемент Q1-10 = K2.DI15 + K2.DI16

  // outputs
  normConrtol(2506, 2535, 0, 0, true);  // 2535.0 = Блокировка ВЭ выключателя Q1-8 = K2.DO1
  normConrtol(2506, 2535, 1, 1, true);  // 2535.1 = Блокировка ЗЭ Q1-8 = K2.DO2
  normConrtol(2506, 2535, 2, 2, true); // 2535.2 = Отключение выключателя Q1-8 = K2.DO3

  normConrtol(2506, 2535, 4, 4, true);  // 2535.4 = Блокировка ВЭ выключателя Q1-10 = K2.DO5
  normConrtol(2506, 2535, 5, 5, true);  // 2535.5 = Блокировка ЗЭ Q1-10 = K2.DO6
  normConrtol(2506, 2535, 6, 6, true); // 2535.6 = Отключение выключателя Q1-10 = K2.DO7
  
  //- 53 -----------------------------------------------------------------------------------------
  // inputs
  diffConrtol(2512, 2530, 0, 1, 0, 1);  // 2530.0 + 2530.1 = Выдв. элемент QS-T3 = KT3.DI1 + KT3.DI2
  diffConrtol(2512, 2530, 2, 3, 2, 3);  // 2530.2 + 2530.3 = Зазем. элемент QS-T3 = KT3.DI3 + KT3.DI4
  
  normConrtol(2512, 2529, 4, 7, false);  // 2529.7 = Защита от дуги в шкафу QS-T3 = KT3.DI5
  normConrtol(2512, 2529, 5, 8, false);  // 2529.8 = Нет опер тока тепловой защиты тр-ра Т3 = KT3.DI6
  normConrtol(2512, 2529, 6, 9, false);  // 2529.9 = Предупредительная  тепловая защита 140 гр С тр-ра Т3 = KT3.DI7
  normConrtol(2512, 2529, 7, 10, false); // 2529.10 = Тепловая авария 155 гр С тр-ра Т3 = KT3.DI8
  normConrtol(2512, 2529, 14, 11, false); // 2525.11 = Защита от дуги в шкафу Q3: 1-"Авария"; 0 -"Работа" = KT3.DI15

  diffConrtol(2512, 2530, 8, 9, 4, 5);   // 2530.4 + 2530.5 = Выключатль Q3 = KT3.DI9 + KT3.DI10
  diffConrtol(2512, 2530, 10, 11, 6, 7); // 2530.6 + 2530.7 = Выдв. элемент Q3 = KT3.DI11 + KT3.DI12
  diffConrtol(2512, 2530, 12, 13, 8, 9); // 2530.8 + 2530.9 = Зазем. элемент QSG3 = KT3.DI13 + KT3.DI14
  
  diffConrtol(2512, 2530, 15, 16, 10, 11); // 2530.10 + 2530.11 = Выдв. элемент Q1-5 = KT3.DI16 + KT3.DI17
  diffConrtol(2513, 2530, 1, 2, 12, 13);   // 2530.12 + 2530.13 = Зазем. элемент Q1-5 = KT3.DI18 + KT3.DI19

  // outputs
  normConrtol(2514, 2536, 0, 0, true); // 2536.0 = Блокировка ВЭ разъединителя QS-T1 = K-T1.DO1
  normConrtol(2514, 2536, 1, 1, true); // 2536.1 = Блокировка ЗЭ разъединителя QSG-T1 = K-T1.DO2
  normConrtol(2514, 2536, 2, 2, true); // 2536.2 = Отключение выключателя Q1 = K-T1.DO3 
  normConrtol(2514, 2536, 3, 3, true); // 2536.3 = Отключение выключателя Q1-3 = K-T1.DO4 
  //- 54 -----------------------------------------------------------------------------------------
  // inputs
  diffConrtol(2508, 2526, 0, 1, 0, 1); // 2526.0 + 2526.1 = Выдв. элемент QS-T1 = KT1.DI1 + KT1.DI2
  diffConrtol(2508, 2526, 2, 3, 2, 3); // 2526.2 + 2526.3 = Зазем. элемент QSG-T1 = KT1.DI3 + KT1.DI4
  
  normConrtol(2508, 2525, 4, 7, false);   // 2525.7 = Защита от дуги в шкафу QS-T1 = KT1.DI5
  normConrtol(2508, 2525, 5, 8, false);   // 2525.8 = Нет опер тока тепловой защиты тр-ра Т1 = KT1.DI6
  normConrtol(2508, 2525, 6, 9, false);   // 2525.9 = Предупредительная  тепловая защита 140 гр С тр-ра Т1 = KT1.DI7
  normConrtol(2508, 2525, 7, 10, false);  // 2525.10 = Тепловая авария 155 гр С тр-ра Т1 = KT1.DI8
  normConrtol(2508, 2525, 14, 11, false); // 2525.11 = Защита от дуги в шкафу Q1: 1-"Авария"; 0 -"Работа" = KT1.DI15

  diffConrtol(2508, 2526, 8, 9, 4, 5);   // 2526.4 + 2526.5 = Выключатль Q1 = KT1.DI9 + KT1.DI10
  diffConrtol(2508, 2526, 10, 11, 6, 7); // 2526.6 + 2526.7 = Выдв. элемент Q1 = KT1.DI11 + KT1.DI12
  diffConrtol(2508, 2526, 12, 13, 8, 9); // 2526.8 + 2526.9 = Зазем. элемент QSG1 = KT1.DI13 + KT1.DI14
  
  diffConrtol(2508, 2526, 15, 16, 10, 11); // 2526.10 + 2526.11 = Выдв. элемент Q1-3 = KT1.DI16 + KT1.DI17
  diffConrtol(2509, 2526, 1, 2, 12, 13);   // 2526.12 + 2526.13 = Зазем. элемент Q1-3 = KT1.DI18 + KT1.DI19
  
  normConrtol(2509, 2533, 3, 4, false); // 2533.4 = Неисправность коммутатора оптического КО2 = KT1.DI20

  // outputs
  normConrtol(2510, 2538, 0, 0, true); // 2538.0 = Блокировка ВЭ разъединителя QS-T3 = K-T3.DO1
  normConrtol(2510, 2538, 1, 1, true); // 2538.1 = Блокировка ЗЭ разъединителя QSG-T3 = K-T3.DO2
  normConrtol(2510, 2538, 2, 2, true); // 2538.2 = Отключение выключателя Q3 = K-T1.DO3 
  normConrtol(2510, 2538, 3, 3, true); // 2538.3 = Отключение выключателя Q1-5 = K-T1.DO4 
 //- 55 -----------------------------------------------------------------------------------------
 // inputs
  diffConrtol(2516, 2528, 0, 1, 0, 1); // 2528.0 + 2528.1 = Выдв. элемент QS-T2 = KT2.DI1 + KT2.DI2
  diffConrtol(2516, 2528, 2, 3, 2, 3); // 2528.2 + 2528.3 = Зазем. элемент QSG-T1 = KT2.DI3 + KT2.DI4
  
  normConrtol(2516, 2527, 4, 7, false);   // 2527.7 = Защита от дуги в шкафу QS-T1 = KT2.DI5
  normConrtol(2516, 2527, 5, 8, false);   // 2527.8 = Нет опер тока тепловой защиты тр-ра Т1 = KT2.DI6
  normConrtol(2516, 2527, 6, 9, false);   // 2527.9 = Предупредительная  тепловая защита 140 гр С тр-ра Т1 = KT2.DI7
  normConrtol(2516, 2527, 7, 10, false);  // 2527.10 = Тепловая авария 155 гр С тр-ра Т1 = KT2.DI8
  normConrtol(2516, 2527, 14, 11, false); // 2527.11 = Защита от дуги в шкафу Q2: 1-"Авария"; 0 -"Работа" = KT2.DI15

  diffConrtol(2516, 2528, 8, 9, 4, 5);   // 2528.4 + 2528.5 = Выключатль Q2 = KT2.DI9 + KT2.DI10
  diffConrtol(2516, 2528, 10, 11, 6, 7); // 2528.6 + 2528.7 = Выдв. элемент Q2 = KT2.DI11 + KT2.DI12
  diffConrtol(2516, 2528, 12, 13, 8, 9); // 2528.8 + 2528.9 = Зазем. элемент QSG2 = KT2.DI13 + KT2.DI14
  
  diffConrtol(2516, 2528, 15, 16, 10, 11); // 2528.10 + 2528.11 = Выдв. элемент Q1-4 = KT2.DI16 + KT2.DI17
  diffConrtol(2517, 2528, 1, 2, 12, 13);   // 2528.12 + 2528.13 = Зазем. элемент Q1-4 = KT2.DI18 + KT2.DI19
  
  normConrtol(2517, 2533, 3, 5, false); // 2533.5 = Неисправность коммутатора оптического КО1 = KT2.DI20

  // outputs
  normConrtol(2518, 2537, 0, 0, true); // 2537.0 = Блокировка ВЭ разъединителя QS-T2 = K-T2.DO1
  normConrtol(2518, 2537, 1, 1, true); // 2537.1 = Блокировка ЗЭ разъединителя QSG-T2 = K-T2.DO2
  normConrtol(2518, 2537, 2, 2, true); // 2537.2 = Отключение выключателя Q3 = K-T1.DO3 
  normConrtol(2518, 2537, 3, 3, true); // 2537.3 = Отключение выключателя Q1-5 = K-T1.DO4 
  
  normConrtol(2517, 2533, 3, 5, false); // 2533.5 = Неисправность коммутатора оптического
  //- 56 -----------------------------------------------------------------------------------------
  diffConrtol(2520, 2532, 0, 1, 0, 1); // 2532.0 + 2532.1 = Выдв. элемент QS-T4 = KT4.DI1 + KT4.DI2
  diffConrtol(2520, 2532, 2, 3, 2, 3); // 2532.2 + 2532.3 = Зазем. элемент QSG-T4 = KT4.DI3 + KT4.DI4
  
  normConrtol(2520, 2531, 4, 7, false);   // 2531.7 = Защита от дуги в шкафу QS-T4 = KT4.DI5
  normConrtol(2520, 2531, 5, 8, false);   // 2531.8 = Нет опер тока тепловой защиты тр-ра Т4 = KT4.DI6
  normConrtol(2520, 2531, 6, 9, false);   // 2531.9 = Предупредительная  тепловая защита 140 гр С тр-ра Т4 = KT4.DI7
  normConrtol(2520, 2531, 7, 10, false);  // 2531.10 = Тепловая авария 155 гр С тр-ра Т4 = KT4.DI8
  normConrtol(2520, 2531, 14, 11, false); // 2531.11 = Защита от дуги в шкафу Q4: 1-"Авария"; 0 -"Работа" = KT4.DI15

  diffConrtol(2520, 2532, 8, 9, 4, 5);   // 2532.4 + 2532.5 = Выключатль Q4 = KT4.DI9 + KT4.DI10
  diffConrtol(2520, 2532, 10, 11, 6, 7); // 2532.6 + 2532.7 = Выдв. элемент Q4 = KT4.DI11 + KT4.DI12
  diffConrtol(2520, 2532, 12, 13, 8, 9); // 2532.8 + 2532.9 = Зазем. элемент QSG4 = KT4.DI13 + KT4.DI14
  
  diffConrtol(2520, 2532, 15, 16, 10, 11); // 2532.10 + 2532.11 = Выдв. элемент Q1-6 = KT4.DI16 + KT4.DI17
  diffConrtol(2521, 2532, 1, 2, 12, 13);   // 2532.12 + 2532.13 = Зазем. элемент Q1-6 = K4.DI18 + KT4.DI19

  normConrtol(2522, 2539, 0, 0, true); // 2539.0 = Блокировка ВЭ разъединителя QS-T4 = K-T4.DO1
  normConrtol(2522, 2539, 1, 1, true); // 2539.1 = Блокировка ЗЭ разъединителя QSG-T4 = K-T4.DO2
  normConrtol(2522, 2539, 2, 2, true); // 2539.2 = Отключение выключателя Q4 = K-T4.DO3 
  normConrtol(2522, 2539, 3, 3, true); // 2539.3 = Отключение выключателя Q1-6 = K-T4.DO4 
}

void controlLogic(uint16_t num)
{
  int i;
  const uint8_t rep[] = {2,4,3,5};
  const uint8_t sound[] = {0,1,2,4};

  controlBit(0+num%2, 0+(num/2)*4, true, true, getBit(2525+num*2, 0) == false && getBit(2525+num*2, 1) == false 
    && getBit(2525+num*2, 5) == false && getBit(2525+num*2, 6) == false 
    && getBit(2526+num*2, 2) == false && getBit(2526+num*2, 3) == false 
    && getBit(2526+num*2, 4) == true  && getBit(2526+num*2, 5) == false 
    && getBit(2526+num*2, 8) == false && getBit(2526+num*2, 9) == false 
    && getBit(2526+num*2, 10) == true && getBit(2526+num*2, 11) == false)
  controlBit(rep[num], 0, true, true, getBit(2525+num*2, 0) == false && getBit(2525+num*2, 1) == false 
    && getBit(2525+num*2, 5) == false && getBit(2525+num*2, 6) == false 
    && getBit(2526+num*2, 2) == false && getBit(2526+num*2, 3) == false 
    && getBit(2526+num*2, 4) == true  && getBit(2526+num*2, 5) == false 
    && getBit(2526+num*2, 8) == false && getBit(2526+num*2, 9) == false 
    && getBit(2526+num*2, 10) == true && getBit(2526+num*2, 11) == false)

  controlBit(0+num%2, 1+(num/2)*4, true, true, getBit(2525+num*2, 0) == false && getBit(2525+num*2, 1) == false 
    && getBit(2525+num*2, 3) == true   && getBit(2525+num*2, 4) == false 
    && getBit(2526+num*2, 0) == true   && getBit(2526+num*2, 1) == false 
    && getBit(2526+num*2, 4) == true   && getBit(2526+num*2, 5) == false  
    && getBit(2526+num*2, 6) == false  && getBit(2526+num*2, 7) == false 
    && getBit(2526+num*2, 10) == true  && getBit(2526+num*2, 11) == false 
    && getBit(2526+num*2, 12) == false && getBit(2526, 13) == false)
  controlBit(rep[num], 1, true, true, getBit(2525+num*2, 0) == false && getBit(2525+num*2, 1) == false 
    && getBit(2525+num*2, 3) == true   && getBit(2525+num*2, 4) == false 
    && getBit(2526+num*2, 0) == true   && getBit(2526+num*2, 1) == false 
    && getBit(2526+num*2, 4) == true   && getBit(2526+num*2, 5) == false 
    && getBit(2526+num*2, 6) == false  && getBit(2526+num*2, 7) == false 
    && getBit(2526+num*2, 10) == true  && getBit(2526+num*2, 11) == false 
    && getBit(2526+num*2, 12) == false && getBit(2526+num*2, 13) == false)
    
  controlBit(0+num%2, 2+(num/2)*4, true, true, getBit(2525+num*2, 5) == false && getBit(2525+num*2, 6) == false 
    && getBit(2526+num*2, 8) == false  && getBit(2526+num*2, 9) == false 
    && getBit(2526+num*2, 2) == false  && getBit(2526+num*2, 3) == false 
    && getBit(2526+num*2, 0) == false  && getBit(2526+num*2, 1) == false 
    && !(getBit(2525+num*2, 7) == true || getBit(2525+num*2, 11) == true) 
    && getBit(2525+num*2, 10) == false && getBit(2525+num*2, 8) == false)

  controlBit(rep[num], 2, true, true, (getBit(2525+num*2, 0) == true && getBit(2525+num*2, 1) == false 
    && getBit(2526+num*2, 0) == false   && getBit(2526+num*2, 1) == false) 
    && !(getBit(2525+num*2, 0) == false && getBit(2525+num*2, 1) == false) 
    && getBit(2525+num*2, 9) == false)

  controlBit(rep[num], 3, true, true, (getBit(2526+num*2, 4) == false && getBit(2526+num*2, 5) == false) 
    && !(getBit(2525+num*2, 0) == false && getBit(2525+num*2, 1) == false) 
    && getBit(2525+num*2, 9) == false   && !(getBit(2526+num*2, 4) == true 
    && getBit(2526+num*2, 5) == false))

  controlBit(sound[num], 19, true, true, Alarms[alarmsActual]->count == 0);


  controlBit(0+num%2, 3, true, true, false);
  controlBit(0+num%2, 7, true, true, false);
  controlBit(0+num%2, 8, true, true, false);
  controlBit(0+num%2, 9, true, true, false);
  controlBit(0+num%2, 10, true, true, false);
  controlBit(0+num%2, 11, true, true, false);
  controlBit(0+num%2, 12, true, true, false);
  controlBit(0+num%2, 13, true, true, false);
  controlBit(0+num%2, 14, true, true, false);
  controlBit(0+num%2, 15, true, true, false);
  controlBit(0+num%2, 16, true, true, false);
  controlBit(0+num%2, 17, true, true, false);
  controlBit(0+num%2, 18, true, true, false);

  controlBit(rep[num], 4, true, true, false);
  controlBit(rep[num], 5, true, true, false);
  controlBit(rep[num], 6, true, true, false);
  controlBit(rep[num], 7, true, true, false);
  controlBit(rep[num], 8, true, true, false);
  controlBit(rep[num], 9, true, true, false);
  controlBit(rep[num], 10, true, true, false);
  controlBit(rep[num], 11, true, true, false);
  controlBit(rep[num], 12, true, true, false);
  controlBit(rep[num], 13, true, true, false);
  controlBit(rep[num], 14, true, true, false);
  controlBit(rep[num], 15, true, true, false);
  controlBit(rep[num], 16, true, true, false);
  controlBit(rep[num], 17, true, true, false);
  controlBit(rep[num], 18, true, true, false);
  if(num >= 2)
  controlBit(rep[num], 19, true, true, false);
}

void alarmLogic(void)
{
  short i;

  switch (getMyIP())
  {
    case 41:  // Panel 43, Panel 44, Gate, Panel 42
      alExContr(alConFailPanel3, (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailPanel4, (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailGate,   (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel2, (getEnable(3) == false) || GetPSBStatus(703));
      alExContr(alConFailAtAllPanel,
        ((getEnable(0) == false) || GetPSBStatus(700)) &&
        ((getEnable(1) == false) || GetPSBStatus(701)) &&
        // ((getEnable(2) == false) || GetPSBStatus(702)) &&
        ((getEnable(3) == false) || GetPSBStatus(703)) 
      );
      alExContr(alConFailShot, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHOT && Panel->flags.isMaster)); 
      alExContr(alConFailShsn, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHSN && Panel->flags.isMaster)); 
      alExContr(alConFailShsnD, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHSND && Panel->flags.isMaster)); 

      break;
    case 42:  // Panel 43, Panel 44, Gate, Panel 41
      alExContr(alConFailPanel3, (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailPanel4, (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailGate,   (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel1, (getEnable(3) == false) || GetPSBStatus(703));
      alExContr(alConFailAtAllPanel,
        ((getEnable(0) == false) || GetPSBStatus(700)) &&
        ((getEnable(1) == false) || GetPSBStatus(701)) &&
        // ((getEnable(2) == false) || GetPSBStatus(702)) &&
        ((getEnable(3) == false) || GetPSBStatus(703)) 
      );
      alExContr(alConFailShot, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHOT && Panel->flags.isMaster)); 
      alExContr(alConFailShsn, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHSN && Panel->flags.isMaster)); 
      alExContr(alConFailShsnD, (!((getEnable(2) == false) || GetPSBStatus(702)) 
        && dMem->Gate->errCon.SHSND && Panel->flags.isMaster)); 
      break;
    case 43:  // K-1, KT-1, KT-3, Panel 44, Panel 42, Panel 41
      alExContr(alConFailDP1,    (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailDP3,    (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailDP4,    (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel4, (getEnable(3) == false) || GetPSBStatus(703));
      alExContr(alConFailPanel2, (getEnable(4) == false) || GetPSBStatus(704));
      alExContr(alConFailPanel1, (getEnable(5) == false) || GetPSBStatus(705));
      alExContr(alConFailAtAllPanel,
        ((getEnable(0) == false) || GetPSBStatus(700)) &&
        ((getEnable(1) == false) || GetPSBStatus(701)) &&
        ((getEnable(2) == false) || GetPSBStatus(702)) &&
        ((getEnable(3) == false) || GetPSBStatus(703)) &&
        ((getEnable(4) == false) || GetPSBStatus(704)) &&
        ((getEnable(5) == false) || GetPSBStatus(705))
      );
      alExContr(alConFailHighLevel, 
        ((getEnable(4) == false) || GetPSBStatus(704)) &&
        ((getEnable(5) == false) || GetPSBStatus(705))
      );
      break;
    case 44:  // K-2, KT-2, KT-4, Panel 43, Panel 41, Panel 42
      alExContr(alConFailDP2,    (getEnable(0) == false) || GetPSBStatus(700));
      alExContr(alConFailDP5,    (getEnable(1) == false) || GetPSBStatus(701));
      alExContr(alConFailDP6,    (getEnable(2) == false) || GetPSBStatus(702));
      alExContr(alConFailPanel4, (getEnable(3) == false) || GetPSBStatus(703));
      alExContr(alConFailPanel1, (getEnable(4) == false) || GetPSBStatus(704));
      alExContr(alConFailPanel2, (getEnable(5) == false) || GetPSBStatus(705));
      alExContr(alConFailAtAllPanel,
        ((getEnable(0) == false) || GetPSBStatus(700)) &&
        ((getEnable(1) == false) || GetPSBStatus(701)) &&
        ((getEnable(2) == false) || GetPSBStatus(702)) &&
        ((getEnable(3) == false) || GetPSBStatus(703)) &&
        ((getEnable(4) == false) || GetPSBStatus(704)) &&
        ((getEnable(5) == false) || GetPSBStatus(705))
      );
      alExContr(alConFailHighLevel, 
        ((getEnable(4) == false) || GetPSBStatus(704)) &&
        ((getEnable(5) == false) || GetPSBStatus(705))
      );
      break;
  }

  if(Alarms[alarmsSHOT]->count != 0 || findAlarms(Alarms[alarmsActual], alConFailShot)) 
    addCrash(alShortEn);
  else deleteCrash(alShortEn);
  if(Alarms[alarmsSHSN]->count != 0 || findAlarms(Alarms[alarmsActual], alConFailShsn)) 
    addCrash(alShsnEn);
  else deleteCrash(alShSnDEn);
  if(Alarms[alarmsSHSND]->count != 0 || findAlarms(Alarms[alarmsActual], alConFailShsnD)) 
    addCrash(alShSnDEn);
  else deleteCrash(alShSnDEn);

  for(i = 0; i < 4; i++) 
  {
    if(getBit(2525+2*i, 9) == true ||  getBit(2525+2*i, 10) == true) 
      addCrash(alTransTemperProtectT1+i);
    else deleteCrash(alTransTemperProtectT1+i);
  }

  if(getBit(2533, 0) == true || getBit(2533, 2) == true) 
    addCrash(alFailOpticalSwitchKO14);
  else deleteCrash(alFailOpticalSwitchKO14);
  if(getBit(2533, 1) == true || getBit(2533, 3) == true) 
    addCrash(alFailOpticalSwitchKO15);
  else deleteCrash(alFailOpticalSwitchKO15);
  if(getBit(2533, 4) == true) 
    addCrash(alFailOpticalSwitchKO2);
  else deleteCrash(alFailOpticalSwitchKO2);
  if(getBit(2533, 5) == true) 
    addCrash(alFailOpticalSwitchKO1);
  else deleteCrash(alFailOpticalSwitchKO1);
}
 
void taskLoop(void)
{
  init();

  Panel->flags.isMaster = true;
  addEvent(alPowerOn1 + getMyIP() - 41);
  Panel->flags.isMaster = false;
  Panel->flags.enableEx = true;

  while(true)
  {
    int i = 0;
    cell_t c;

    clearRRScreens();
    switch (PSW[CURRENT_SCREEN])
    {
      case scrMnemotic: screenMnemotic(); break;
      case scrConfAlarms:
        screenConfCrash();
        selectDeviceInCrashAndEvent();
        break;
      case scrZVU:   screenZvu();   break;
      case scrBKI:   screenBkif();  break;
      case scrSHOT:  screenShot();  break;
      case scrSHSN:  screenShsn();  break;
      case scrCrash:
        screenCrash();
        selectDeviceInCrashAndEvent();
        break;
      case scrEvent:
        screenEvent();
        selectDeviceInCrashAndEvent();
        break;

      default:
        break;
    }
    fillRRScreens();

    fillCrash();

    updateScreen();
    controlMenu();

    for(i = 0; i < 6; i++) connectionFaultHandler(i);

    handlerLogic();
    
    if (Panel->flags.chooseTestMode == false)
    {
      switch (getMyIP())
      {
        case 41:
        case 42:
          break;
        case 43:
          controlLogic(0);  //T1
          controlLogic(2);  //T3
          break;
        case 44:
          controlLogic(1);  //T2
          controlLogic(3);  //T4
          break;
      }
    }
    else
    {

    }
    
    alarmLogic();

    updatePFW();
    getTime();

    if(GetAdminLevelAvtorisation != Panel->flags.StateAdminAccessOld)
    {
      Panel->flags.StateAdminAccessOld = !Panel->flags.StateAdminAccessOld;
      if(GetAdminLevelAvtorisation) 
      {
        addEvent(alOpenAdminAccess1 + getMyIP() - 41);
        SetUserLevelAvtorisation;
      }
    }
    if(GetUserLevelAvtorisation != Panel->flags.StateUserAccessOld)
    {
      Panel->flags.StateUserAccessOld = !Panel->flags.StateUserAccessOld;
      if(GetUserLevelAvtorisation) addEvent(alOpenUserAccess1 + getMyIP() - 41);
    }
    Panel->flags.notUserAccess = !GetUserLevelAvtorisation;
    Panel->flags.notAdminAccess = !GetAdminLevelAvtorisation;
    if(Panel->flags.notLevelAccess)
    {
      SHOW_ERROR_MSG(numTE_IncorrectLevelAccess);
      Panel->flags.notLevelAccess = false;
    }

    Delay(20);
  }
}
