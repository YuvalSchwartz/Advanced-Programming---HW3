
#ifndef MAXHEAP_H_
#define MAXHEAP_H_

#include "Defs.h"

MaxHeap    		 CreateHeap	  			 (copyFunction, freeFunction, printFunction, equalFunction, int, char*);
status     		 destroyHeap		     (MaxHeap);
status     		 insertToHeap		     (MaxHeap, element);
status     		 printHeap		     	 (MaxHeap);
element    		 PopMaxHeap		     	 (MaxHeap);
element    		 TopMaxHeap    			 (MaxHeap);
char*      		 getHeapId    			 (MaxHeap);
int 	   		 getHeapCurrentSize      (MaxHeap);
//extras:
int		   		 getHeapMaxCapacity      (MaxHeap);
copyFunction	 getHeapCopyFunction     (MaxHeap);
freeFunction     getHeapFreeFunction     (MaxHeap);
printFunction    getHeapPrintFunction    (MaxHeap);
equalFunction    getHeapEqualFunction    (MaxHeap);


#endif /* MAXHEAP_H_ */
