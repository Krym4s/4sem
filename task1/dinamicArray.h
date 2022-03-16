#ifndef DINAMIC_ARRAY
#define DINAMIC_ARRAY
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"

struct dinamic_array;

enum ArrayErrors
{
  NO_ERRORS,
  BAD_ALLOC,
  OUT_OF_BORDERS,
  FUNCTION_ERROR,
  IS_EMPTY,
  EXIST
};

unsigned GetArraySize (struct dinamic_array* array);
unsigned GetElementSize (struct dinamic_array* array);

enum ArrayErrors ConstructArray (struct dinamic_array** array, unsigned size, unsigned elementSize);
enum ArrayErrors DestructArray (struct dinamic_array* array);
void Dump (struct dinamic_array* array, enum ArrayErrors (*print)(struct dinamic_array*, unsigned, void *));

void* GetElement (struct dinamic_array* array, unsigned index);
enum ArrayErrors InsertElement (struct dinamic_array* array, unsigned index, void* element);
enum ArrayErrors foreach (struct dinamic_array* array, enum ArrayErrors (*hook)(struct dinamic_array*, unsigned, void*), void* resources);

enum ArrayErrors ResizeBuffer (struct dinamic_array* array, unsigned size);
void* MyMalloc (unsigned size);
void* MyRealloc (void* ptr, unsigned size);

#endif // DINAMIC_ARRAY