#include "LinkedList.h"

typedef struct node_s{
	element elem;
	struct node_s *next;
}node;

struct linked_list_s{
	node *head;
	copyFunction ll_element_copy;
	freeFunction ll_element_free;
	printFunction ll_element_print;
	equalFunction ll_elements_equal;
	equalFunction ll_element_and_key_equal;
};

LinkedList createLinkedList(copyFunction ecf, freeFunction eff, printFunction epf, equalFunction eef, equalFunction eakef) {
	if (ecf == NULL || eff == NULL || epf == NULL || eef == NULL || eakef == NULL) return NULL;
	LinkedList l = (LinkedList) malloc (sizeof(struct linked_list_s));
	if (l == NULL) return NULL;
	l->head = NULL;
	l->ll_element_copy = ecf;
	l->ll_element_free = eff;
	l->ll_element_print = epf;
	l->ll_elements_equal = eef;
	l->ll_element_and_key_equal = eakef;
	return l;
}

status destroyList(LinkedList l) {
	if (l == NULL) return null_pointer_failure;
	node* temp;
	node* nd_ptr = (*l).head;
	while (nd_ptr != NULL) {
		if (nd_ptr->elem != NULL) l->ll_element_free(nd_ptr->elem);
		temp = nd_ptr->next;
		free(nd_ptr);
		nd_ptr = temp;
	}
	free(l);
	l = NULL;
	return success;
}

status appendNode(LinkedList l, element e) {
	if (l == NULL || e == NULL) return null_pointer_failure;
	node* new_nd = (node*) malloc (sizeof(node));
	if (new_nd == NULL) {
		destroyList(l);
		return memory_failure;
	}
	new_nd->elem = l->ll_element_copy(e);
	if (new_nd->elem == NULL) return memory_failure;
	new_nd->next = NULL;
	node* nd_ptr  = (*l).head;
	if (nd_ptr == NULL) {
		l->head = new_nd;
	}
	else {
		while (nd_ptr->next != NULL) nd_ptr = nd_ptr->next;
		nd_ptr->next = new_nd;
	}
	return success;
}

status deleteNode(LinkedList l, element e) {
	if (l == NULL || e == NULL) return null_pointer_failure;
	node* temp;
	node* nd_ptr = (*l).head;
	if (l->ll_elements_equal(nd_ptr->elem, e) == 0) {
		temp = nd_ptr;
		l->head = l->head->next;
		l->ll_element_free(temp->elem);
		free(temp);
		temp = NULL;
		return success;
	}
	else {
		while (nd_ptr->next != NULL) {
			if (l->ll_elements_equal(nd_ptr->next->elem, e) == 0) {
				temp = nd_ptr->next;
				nd_ptr->next = nd_ptr->next->next;
				l->ll_element_free(temp->elem);
				free(temp);
				temp = NULL;
				return success;
			}
			nd_ptr = nd_ptr->next;
		}
	}
	return remove_failure;
}

status displayList(LinkedList l) {
	if (l == NULL) return null_pointer_failure;
	node* nd_ptr = (*l).head;
	if (nd_ptr == NULL) return print_failure;
	while (nd_ptr != NULL) {
		l->ll_element_print(nd_ptr->elem);
		nd_ptr = nd_ptr->next;
	}
	return success;
}

element searchByKeyInList(LinkedList l, element information) {
	if (l == NULL || information == NULL) return NULL;
	node* nd_ptr = (*l).head;
	while (nd_ptr != NULL) {
		if (l->ll_element_and_key_equal(nd_ptr->elem, information) == 0) {
			return nd_ptr->elem;
		}
		nd_ptr = nd_ptr->next;
	}
	return NULL;
}

