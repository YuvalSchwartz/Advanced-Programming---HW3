
#ifndef DEFS_H_
#define DEFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_INPUT_SIZE 300

typedef enum {me, others} which_to_enter;

typedef enum {false, true} bool;

typedef enum {add_failure, remove_failure, memory_failure, print_failure, null_pointer_failure, success} status;

typedef void * element;

typedef struct linked_list_s* LinkedList;

typedef struct max_heap_s* MaxHeap;


typedef element(*copyFunction) (element);

typedef status (*freeFunction) (element);

typedef status (*printFunction) (element);

typedef int(*equalFunction) (element, element);//1 first bigger, 0 - same, -1 second bigger

typedef char*(*getCategoryFunction)(element);

typedef int(*getAttackFunction)(element firstElem ,element secondElem ,int* attackFirst,int* attackSecond); // return the first attack - second attack . also produce the attack of each element, The function transmits the information through the pointers received in the function.

typedef struct Type Type;

typedef struct BiologicalInformation BiologicalInformation;

typedef struct Pokemon Pokemon;

#endif /* DEFS_H_ */
