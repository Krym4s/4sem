#include "dinamicArray.h"

const unsigned GoodSize = 10; 

extern enum ArrayErrors ConstructArray (struct dinamic_array** array, unsigned size, unsigned elementSize);
extern enum ArrayErrors DestructArray (struct dinamic_array* array);

extern void* GetElement (struct dinamic_array* array, unsigned index);
extern enum ArrayErrors InsertElement (struct dinamic_array* array, unsigned index, void* element);
extern enum ArrayErrors foreach (struct dinamic_array* array, enum ArrayErrors (*hook)(struct dinamic_array*, unsigned, void*), void* resources);

extern enum ArrayErrors ResizeBuffer (struct dinamic_array* array, unsigned size);
extern void* MyMalloc (unsigned size);
extern void* MyRealloc (void* ptr, unsigned size);

extern unsigned GetArraySize (struct dinamic_array* array);
extern unsigned GetElementSize (struct dinamic_array* array);

void Dump (struct dinamic_array* array, enum ArrayErrors (*print)(struct dinamic_array*, unsigned, void *));

enum ArrayErrors print (struct dinamic_array* array, unsigned idx, void * element)
{
    printf ("\t\t[%d]\n", *(int*)GetElement (array, idx));
    return NO_ERRORS;
}

enum ArrayErrors isOdd  (struct dinamic_array* array, unsigned idx, void* res)
{
  return BAD_ALLOC;
}

enum ArrayErrors search (struct dinamic_array* array, unsigned idx, void* res)
{
  if (idx == 0)
    *(int*)(res + sizeof(int)) = 0;

  int* element = (int*)GetElement (array, idx) ;

  if (element != NULL)
  {
    if (*element == *(int*)res)
      (*(int*)(res + sizeof(int)))++;
  }
  else
    return OUT_OF_BORDERS;

  return NO_ERRORS;
}

int main()
{
  //MyMalloc test
  int* ptr;
  for (unsigned idx = 0; idx < 49 ; idx ++){
    ptr = (int*)MyMalloc(sizeof(int));
    if (ptr == NULL)
      printf ("Test 1 failed.%d\n", idx);
    else
      free (ptr);
  }

  ptr = (int*) MyMalloc (sizeof(int));
  if (ptr != NULL)
    printf ("Test 2 failed.\n");

  //MYRealloc test

  ptr = (int*)MyMalloc(sizeof(int));

  for (unsigned idx = 0; idx < 48 ; idx ++){
    ptr = (int*)MyRealloc(ptr, sizeof(int));
    if (ptr == NULL)
      printf ("Test 3 failed.%d\n", idx);
  }

  int* NewPtr = (int*) MyRealloc (ptr, sizeof(int));
  if (NewPtr != NULL)
    printf ("Test 4 failed.\n");

  free (ptr);

  //Construct test
  struct dinamic_array *array, *badArray;

  if (ConstructArray (&array, 10, sizeof(int)) != NO_ERRORS)
    printf ("Test 5 failed.\n");

  for (unsigned idx = 0; idx < 47 ; idx ++){
    ptr = (int*)MyMalloc(sizeof(int));
    free (ptr);
  }

    if (ConstructArray (&badArray, 1,1) != BAD_ALLOC)
        printf ("Test 6 failed.\n");

    for (unsigned idx = 0; idx < 48 ; idx ++){
        ptr = (int*)MyMalloc(sizeof(int));
        free (ptr);
    }

    if (ConstructArray (&badArray, 1,1) != BAD_ALLOC)
        printf ("Test 7 failed.\n");

    //DestructArray test
    if (DestructArray (array) != NO_ERRORS)
        printf ("Test 8 failed.\n");

    //GetArraySize test
    ConstructArray (&array, GoodSize, sizeof(int));
    if (GetArraySize (array) != GoodSize)
        printf ("Test 9 failed.\n");

    //GetElementSize test
    if (GetElementSize (array) != sizeof(int))
        printf ("Test 10 failed.\n");
    
    //ResizeBuffer
  
    if (ResizeBuffer (array, 5) != NO_ERRORS)
        printf ("Test 11 failed.\n");

    if (GetArraySize (array) != 5)
        printf ("Test 12 failed.\n");

    if (ResizeBuffer (array, 10) != NO_ERRORS)
        printf ("Test 13 failed.\n");

    if (GetArraySize (array) != 10)
        printf ("Test 14 failed.\n");

    for (unsigned idx = 0; idx < 43 ; idx ++){
        ptr = (int*)MyMalloc(sizeof(int));
        free (ptr);
    }

    ConstructArray (&badArray, GoodSize, sizeof(int));
    if (ResizeBuffer (badArray, 8) != BAD_ALLOC)
        printf ("Test 15 failed.\n");

    //InsertElement tests
    int ints[10] = {1,5,2,9,5,2,7,1,0,3};
    for (unsigned idx = 0; idx < 10; idx++)
        if (InsertElement (array, idx, ints + idx) != NO_ERRORS)
        printf ("Test 16 failed.\n");
  
    if (InsertElement (array, 11, ints) != OUT_OF_BORDERS)
        printf ("Test 17 failed.\n");

    //Dummp test
    Dump (array, print);

    //Get Element tests

    if (GetElement (array, 10) != NULL)
        printf ("Test 18 failed.\n");
  
    if (*(int*)GetElement (array, 0) != 1)
        printf ("Test 19 failed.\n");

    //foreach test
    int resource[2] = {1,100};
    if (foreach (array, search, resource) != NO_ERRORS)
        printf ("Test 20 failed.\n");

    if (resource[1] != 2)
        printf ("Test 21 failed.\n");

    if (foreach (array, isOdd, NULL) != FUNCTION_ERROR)
        printf ("Test 22 failed.\n");

    DestructArray (array);
    DestructArray (badArray);

  return 0;
}