#include "BattleByCategory.h"
#include "LinkedList.h"
#include "MaxHeap.h"



//Max Heap functions that Linked List would receive:

element MaxHeapCopy(element original) {
	if (original == NULL) return NULL;

	copyFunction original_copy_function = getHeapCopyFunction(((MaxHeap)(original)));
	freeFunction original_free_function = getHeapFreeFunction(((MaxHeap)(original)));
	printFunction original_print_function = getHeapPrintFunction(((MaxHeap)(original)));
	equalFunction original_equal_function = getHeapEqualFunction(((MaxHeap)(original)));
	int original_max_capacity = getHeapMaxCapacity(((MaxHeap)(original)));
	char* original_name = getHeapId(((MaxHeap)(original)));

	MaxHeap copy = CreateHeap(original_copy_function,
							  original_free_function,
							  original_print_function,
							  original_equal_function,
							  original_max_capacity,
							  original_name);
	if (copy == NULL) return NULL;

	status condition;
	int i, j, original_current_size = getHeapCurrentSize(((MaxHeap)(original)));
	element temp[original_current_size];
	for (i = 0; i < original_current_size; i++) {
		temp[i] = PopMaxHeap(((MaxHeap)(original)));
		condition = insertToHeap(copy, temp[i]);
		if (condition != success) {
			for (j = 0; j <= i; j++)
				original_free_function(temp[i]);
			destroyHeap(copy);
			return NULL;
		}
	}
	for (i = 0; i < original_current_size; i++) {
		condition = insertToHeap(((MaxHeap)(original)), temp[i]);
		if (condition != success) {
			for (j = original_current_size - 1; j >= i; j--)
				original_free_function(temp[i]);
			destroyHeap(copy);
			return NULL;
		}
		original_free_function(temp[i]);
	}

	return copy;
}

int MaxHeapEqual(element a, element b) {
	if (a == NULL || b == NULL) return -1;

	if (strcmp(getHeapId(((MaxHeap)(a))), getHeapId(((MaxHeap)(b)))) != 0) return -1;
	if (getHeapMaxCapacity(((MaxHeap)(a))) != getHeapMaxCapacity(((MaxHeap)(b)))) return -1;
	if (getHeapCurrentSize(((MaxHeap)(a))) != getHeapCurrentSize(((MaxHeap)(b)))) return -1;
	if (getHeapCopyFunction(((MaxHeap)(a))) != getHeapCopyFunction(((MaxHeap)(b)))) return -1;
	if (getHeapFreeFunction(((MaxHeap)(a))) != getHeapFreeFunction(((MaxHeap)(b)))) return -1;
	if (getHeapPrintFunction(((MaxHeap)(a))) != getHeapPrintFunction(((MaxHeap)(b)))) return -1;
	if (getHeapEqualFunction(((MaxHeap)(a))) != getHeapEqualFunction(((MaxHeap)(b)))) return -1;

	status condition;
	int flag = 0, i, j, k, original_current_size = getHeapCurrentSize(((MaxHeap)(a)));
	element temp1[original_current_size], temp2[original_current_size];
	for (i = 0; i < original_current_size; i++) {
		temp1[i] = PopMaxHeap(((MaxHeap)(a)));
		temp2[i] = PopMaxHeap(((MaxHeap)(b)));
		if (getHeapEqualFunction(((MaxHeap)(a)))(temp1[i], temp2[i]) != 0) {
			flag = -1;
			break;
		}
	}
	for (j = 0; j <= i; j++) {
		condition = insertToHeap(((MaxHeap)(a)), temp1[i]);
		if (condition != success) {
			for (k = 0; k <= j; k++) {
				getHeapFreeFunction(((MaxHeap)(a)))(temp1[i]);
				getHeapFreeFunction(((MaxHeap)(a)))(temp2[i]);
			}
			return flag;
		}
		getHeapFreeFunction(((MaxHeap)(a)))(temp1[i]);
		condition = insertToHeap(((MaxHeap)(b)), temp2[i]);
		if (condition != success) {
			for (k = 0; k <= j; k++)
				getHeapFreeFunction(((MaxHeap)(a)))(temp2[i]);
			return flag;
		}
		getHeapFreeFunction(((MaxHeap)(a)))(temp2[i]);
	}

	return flag;
}

int MaxHeapKeyEqual(element heap, element name) {
	if (heap == NULL || name == NULL) return -1;
	if (strcmp(getHeapId(((MaxHeap)(heap))), ((char*)(name))) != 0) return -1;
	return 0;
}

status MaxHeapFree(element mh) {
	if (mh == NULL) return null_pointer_failure;
	return destroyHeap(((MaxHeap)(mh)));
}

status MaxHeapPrint(element mh) {
	if (mh == NULL) return null_pointer_failure;
	return printHeap(((MaxHeap)(mh)));
}



struct battle_s{
	int b_capacity;
	int b_num_of_categories;
	char* b_categories;
	equalFunction b_elements_equal;
	copyFunction b_element_copy;
	freeFunction b_element_free;
	getCategoryFunction b_element_category;
	getAttackFunction b_element_attack;
	printFunction b_element_print;

	LinkedList b_ll;
};

Battle createBattleByCategory(int capacity, int numberOfCategories, char* categories, equalFunction equalElement, copyFunction copyElement, freeFunction freeElement, getCategoryFunction getCategory, getAttackFunction getAttack, printFunction printElement) {
	if (categories == NULL || equalElement == NULL || copyElement == NULL || freeElement == NULL || getCategory == NULL || getAttack == NULL || printElement == NULL) return NULL;

	Battle b = (Battle) malloc(sizeof(struct battle_s));
	if (b == NULL) return NULL;

	b->b_categories = (char *) malloc ((strlen(categories) + 1) * sizeof(char));
	if (b->b_categories == NULL) {
		free(b);
		return NULL;
	}
	strcpy(b->b_categories, categories);
	b->b_capacity = capacity;
	b->b_num_of_categories = numberOfCategories;
	b->b_elements_equal = equalElement;
	b->b_element_copy = copyElement;
	b->b_element_free = freeElement;
	b->b_element_category = getCategory;
	b->b_element_attack = getAttack;
	b->b_element_print = printElement;
	b->b_ll = createLinkedList(MaxHeapCopy, MaxHeapFree, MaxHeapPrint, MaxHeapEqual, MaxHeapKeyEqual);
	if (b->b_ll == NULL) {
		free(b->b_categories);
		free(b);
		return NULL;
	}
	MaxHeap mh;
	status condition;
	char temp_categories[strlen(b->b_categories) + 1];
	strcpy(temp_categories, b->b_categories);
	char *token;
	token = strtok(temp_categories, ",");
	int i;
	for (i = 0; i < numberOfCategories; i++) {
		mh = CreateHeap(copyElement, freeElement, printElement, equalElement, capacity, token);
		if (mh == NULL) {
			free(b->b_categories);
			destroyList(b->b_ll);
			free(b);
			return NULL;
		}
		condition = appendNode(b->b_ll, mh);
		if (condition == memory_failure) {
			free(b->b_categories);
			destroyList(b->b_ll);
			free(b);
			return NULL;
		}
		else if (condition == null_pointer_failure) printf("Null pointer failure.");
		token = strtok(NULL, ",");
		destroyHeap(mh);
	}
	return b;
}

status destroyBattleByCategory(Battle b) {
	if (b == NULL) return null_pointer_failure;
	if (b->b_ll != NULL) destroyList(b->b_ll);
	b->b_ll = NULL;
	if (b->b_categories != NULL) free(b->b_categories);
	b->b_categories = NULL;
	free(b);
	b = NULL;
	return success;
}

status insertObject(Battle b, element elem) {
	if (b == NULL || elem == NULL) return null_pointer_failure;
	MaxHeap relevant_category = searchByKeyInList(b->b_ll, b->b_element_category(elem));
	if (relevant_category == NULL) return add_failure;
	element temp_copy = b->b_element_copy(elem);
	if (temp_copy == NULL) return memory_failure;
	status condition = insertToHeap(relevant_category, temp_copy);
	if (condition != success) return condition;
	return b->b_element_free(temp_copy);
}

void displayObjectsByCategories(Battle b) {
	if (b == NULL) return;
	displayList(b->b_ll);
}

element removeMaxByCategory(Battle b,char* category) {
	if (b == NULL || category == NULL) return NULL;
	MaxHeap relevant_category = searchByKeyInList(b->b_ll, category);
	if (relevant_category == NULL) return NULL;
	return PopMaxHeap(relevant_category);
}

int getNumberOfObjectsInCategory(Battle b,char* category) {
	if (b == NULL || category == NULL) return -1;
	MaxHeap relevant_category = searchByKeyInList(b->b_ll, category);
	if (relevant_category == NULL) return -1;
	return getHeapCurrentSize(relevant_category);
}

element fight(Battle b,element elem) {
	if (b == NULL || elem == NULL) return NULL;
	int potential_fighter_attack, chosen_one_attack, users_choice_attack, potential_result, best_result = INT_MIN;
	MaxHeap potential_category;
	element potential_fighter, chosen_one = NULL;
	char temp_categories[strlen(b->b_categories) + 1];
	strcpy(temp_categories, b->b_categories);
	char *token;
	token = strtok(temp_categories, ",");
	int i;
	for (i = 0; i < b->b_num_of_categories; i++) {
		potential_category = searchByKeyInList(b->b_ll, token);
		if (potential_category != NULL) {
			potential_fighter = TopMaxHeap(potential_category);
			if (potential_fighter != NULL) {
				potential_result = b->b_element_attack(potential_fighter, elem, &potential_fighter_attack, &users_choice_attack);
				if (potential_result > best_result) {
					if (chosen_one != NULL)
						b->b_element_free(chosen_one);
					chosen_one = potential_fighter;
					chosen_one_attack = potential_fighter_attack;
					best_result = potential_result;
				}
				else
					b->b_element_free(potential_fighter);
			}
		}
		token = strtok(NULL, ",");
	}
	if (chosen_one == NULL) {
		printf("There are no Pokemons to fight against you.\n");
		return NULL;
	}
	printf("The final battle between:\n");
	b->b_element_print(elem);
	printf("In this battle his attack is :%d\n\n", users_choice_attack);
	printf("against ");
	b->b_element_print(chosen_one);
	printf("In this battle his attack is :%d\n\n", chosen_one_attack);
	if (best_result < 0) {                  //club lost
		printf("THE WINNER IS:\n");
		b->b_element_print(elem);
		b->b_element_free(chosen_one);
		return elem;
	}
	else if (best_result > 0) {             //club won
		printf("THE WINNER IS:\n");
		b->b_element_print(chosen_one);
		return chosen_one;
	}
	else {                                  //draw
		printf("IT IS A DRAW.\n");
		return chosen_one;
	}
}
