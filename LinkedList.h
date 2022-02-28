
#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "Defs.h"

LinkedList    createLinkedList	   (copyFunction, freeFunction, printFunction, equalFunction, equalFunction);
status        destroyList		   (LinkedList);
status        appendNode		   (LinkedList, element);
status        deleteNode		   (LinkedList, element);
status 	      displayList		   (LinkedList);
element       searchByKeyInList    (LinkedList, element);

#endif /* LINKEDLIST_H_ */
