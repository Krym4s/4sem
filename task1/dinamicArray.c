#include "dinamicArray.h"

const unsigned probability = 50;
const double EnlargeCoef = 2;
static unsigned turn = 0;

void* MyMalloc (unsigned size)
{
  turn++;

  if (turn % probability == 0)
    return NULL;

  return malloc (size);
}

void* MyRealloc (void* ptr, unsigned size)
{
  assert (ptr);
  turn++;

  if (turn % probability == 0)
    return NULL;

  return realloc (ptr, size);
}

enum ArrayErrors DestructArray (struct dinamic_array* array)
{
  assert (array);

  free (array->buffer);
  array->capacity = 0;
  array->elemSize = 0;
  return NO_ERRORS;
}

enum ArrayErrors ConstructArray (struct dinamic_array* array, unsigned size, unsigned elementSize)
{
  assert (array);
  
  array->elemSize = elementSize;
  array->buffer = MyMalloc (size * elementSize);
  if (!array->buffer)
    return BAD_ALLOC;

  array->capacity = size;

  return NO_ERRORS;
}

enum ArrayErrors ResizeBuffer (struct dinamic_array* array, unsigned size)
{
  assert (array);
  assert (array->buffer);

  void* tempPtr = MyRealloc(array->buffer, size * array->elemSize);
  if (!tempPtr)
    return BAD_ALLOC;

  array->capacity = size;
  array->buffer = tempPtr;
  return NO_ERRORS;
}

enum ArrayErrors InsertElement (struct dinamic_array* array, unsigned index, void* element)
{
  assert (array);
  assert (array->buffer);

  if (index >= array->capacity)
    return OUT_OF_BORDERS;

  memcpy (array->buffer + index * array->elemSize, element, array->elemSize);
  return NO_ERRORS;
}

void* GetElement (struct dinamic_array* array, unsigned index)
{
  assert (array);
  assert (array->buffer);

  if (index >= array->capacity)
    return NULL;

  return array->buffer + index * array->elemSize;
}

enum ArrayErrors foreach (struct dinamic_array* array, enum ArrayErrors (*hook)(struct dinamic_array*, unsigned, void*), void* resources)
{
  assert (array);
  assert (hook);

  for (unsigned idx = 0; idx < array->capacity; idx++)
  {
    enum ArrayErrors error;
    error = hook (array, idx, resources);
    if (error != NO_ERRORS)
      return FUNCTION_ERROR;
  }
  return NO_ERRORS;
}

void Dump (struct dinamic_array* array, int (*print)(void *))
{
  assert (array);
  assert (print);

  printf ("Dinamic array [%p]\n{\n", array);
  printf ("\tcapacity - %u\n", array->capacity);
  printf ("\telenSize - %u\n", array->elemSize);

  printf ("\tbuffer - [%p]\n", array->buffer);
  for (unsigned idx = 0; idx < array->capacity; idx++)
    print (array->buffer + array->elemSize * idx);

  printf ("}\n");
}