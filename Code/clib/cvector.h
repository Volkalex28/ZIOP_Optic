#ifndef __CVECTOR_H__
#define __CVECTOR_H__

#include <stdint.h>

#define CVECTOR_INIT_CAPACITY 4

typedef struct
{
  void ** pdata;
  size_t data_size;
  size_t size;
  size_t capacity;
} cvector_t;

void cvector_init(cvector_t * const pcvector, const size_t data_size);
size_t cvector_size(cvector_t * const pcvector);
uint8_t cvector_push(cvector_t * const pcvector, void * const pdata);
uint8_t cvector_insert(cvector_t * const pcvector, const uint16_t index, void * const pdata);
void cvector_set(cvector_t * const pcvector, const uint16_t index, void * const pdata);
void * cvector_get(cvector_t * const pcvector, const uint16_t index);
uint8_t cvector_delete(cvector_t * const pcvector, const uint16_t index);
void cvector_clear(cvector_t * const pcvector);

#endif // __CVECTOR_H__
