/**
 * @file panel.c
 * @author VAlex28 (anan20000228@gmail.com)
 * @brief 
 * 
 */

#include "panel.h"
#include "manager.h"

Panel_t * Panel;

uint8_t getMyIP(void) { 
  cell_t c; c.type = memPFW; c.number = 87;
  return read(c).value; 
}
