#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "cvector.h"

void cvector_resize(cvector_t * const pcvector, const size_t new_size);

void cvector_init(cvector_t * const pcvector, const size_t data_size)
{
  pcvector->capacity = CVECTOR_INIT_CAPACITY;
  pcvector->size = 0;
  pcvector->data_size = data_size;
  cvector_resize(pcvector, pcvector->capacity);
}

size_t cvector_size(cvector_t * const pcvector)
{
  return pcvector->size;
}

uint8_t cvector_push(cvector_t * const pcvector, void * const pdata)
{
  return cvector_insert(pcvector, pcvector->size, pdata);
}

uint8_t cvector_insert(cvector_t * const pcvector, const uint16_t index, void * const pdata)
{
  size_t i;
  
  if(index > pcvector->size || pdata == NULL)
  {
    return 0;
  }
  
  if(pcvector->size >= pcvector->capacity)
  {
    cvector_resize(pcvector, pcvector->capacity * 1.5f);
  }
  pcvector->pdata[pcvector->size] = (void *)malloc(pcvector->data_size);
  
  for(i = pcvector->size; i > index; i--)
  {
    memcpy(pcvector->pdata[i], pcvector->pdata[i-1], pcvector->data_size);
  }
  memcpy(pcvector->pdata[index], pdata, pcvector->data_size);
   
  return (++pcvector->size);
}

void cvector_set(cvector_t * const pcvector, const uint16_t index, void * const pdata)
{
  memcpy(pcvector->pdata[index], pdata, pcvector->data_size);
}

void * cvector_get(cvector_t * const pcvector, const uint16_t index)
{
  if(index >= pcvector->size)
  {
    return NULL;
  }
  return pcvector->pdata[index];
}

uint8_t cvector_delete(cvector_t * const pcvector, const uint16_t index)
{
  size_t i;

  if(index >= pcvector->size || pcvector->size == 0)
  {
    return 1;
  }
   
  free(cvector_get(pcvector, index));
   
  for(i = index; i < pcvector->size - 1; i++)
  {
    pcvector->pdata[i] = pcvector->pdata[i + 1];
  }
   
  pcvector->size--;
  if(pcvector->size * 1.5f < pcvector->capacity)
  {
    cvector_resize(pcvector, pcvector->size * 1.5f);
  }
  return 0;
}

void cvector_clear(cvector_t * const pcvector)
{
  size_t i;
  for(i = 0; i < pcvector->size; i++)
  {
    free(pcvector->pdata[i]);
  }
  pcvector->size = 0;
  free(pcvector->pdata);
}


void cvector_resize(cvector_t * const pcvector, const size_t new_size)
{
  pcvector->capacity = new_size;
  pcvector->pdata = (void **)realloc(pcvector->pdata, new_size * sizeof(void *));
  assert(pcvector->pdata != NULL);
}
