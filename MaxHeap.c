#include "MaxHeap.h"

struct max_heap_s{
	element *elements_arr;
	char* heap_name;
	int max_size;
	int current_size;
	copyFunction mh_element_copy;
	freeFunction mh_element_free;
	printFunction mh_element_print;
	equalFunction mh_elements_equal;
};

static void HeapIncreaseLastElement(MaxHeap mh) {
	int element_index = mh->current_size - 1;
	int parent_index = (element_index - 1)/2;
	element temp;
	while (element_index > 0 && mh->mh_elements_equal(mh->elements_arr[parent_index], mh->elements_arr[element_index]) == -1) {
		temp = mh->elements_arr[element_index];
		mh->elements_arr[element_index] = mh->elements_arr[parent_index];
		mh->elements_arr[parent_index] = temp;
		element_index = parent_index;
		parent_index = element_index/2;
	}
}

static void MaxHeapify(MaxHeap mh, int index) {
	int right = (index + 1) * 2, left = right - 1, largest;
	if (left < mh->current_size && mh->mh_elements_equal(mh->elements_arr[left], mh->elements_arr[index]) == 1) largest = left;
	else largest = index;
	if (right < mh->current_size && mh->mh_elements_equal(mh->elements_arr[right], mh->elements_arr[largest]) == 1) largest = right;
	element temp;
	if (largest != index) {
		temp = mh->elements_arr[index];
		mh->elements_arr[index] = mh->elements_arr[largest];
		mh->elements_arr[largest] = temp;
		MaxHeapify(mh, largest);
	}
}

MaxHeap CreateHeap(copyFunction ecf, freeFunction eff, printFunction epf, equalFunction eef, int max_capacity, char* mh_name) {
	if (ecf == NULL || eff == NULL || epf == NULL || eef == NULL || mh_name == NULL) return NULL;
	MaxHeap mh = (MaxHeap) malloc (sizeof(struct max_heap_s));
	if (mh == NULL) return NULL;
	mh->max_size = max_capacity;
	mh->elements_arr = (element *) malloc (sizeof(element) * mh->max_size);
	if (mh->elements_arr == NULL) {
		free(mh);
		return NULL;
	}
	int i;
	for (i = 0; i < mh->max_size; i++) mh->elements_arr[i] = NULL;
	mh->heap_name = (char *) malloc (sizeof(char) * (strlen(mh_name) + 1));
	if (mh->heap_name == NULL) {
		free(mh->elements_arr);
		free(mh);
		return NULL;
	}
	strcpy(mh->heap_name, mh_name);
	mh->current_size = 0;
	mh->mh_element_copy = ecf;
	mh->mh_element_free = eff;
	mh->mh_element_print = epf;
	mh->mh_elements_equal = eef;
	return mh;
}

status destroyHeap(MaxHeap mh) {
	if (mh == NULL) return null_pointer_failure;
	if (mh->heap_name != NULL) {
		free(mh->heap_name);
		mh->heap_name = NULL;
	}
	int i;
	for (i = 0; i < mh->max_size; i++) {
		if (mh->elements_arr[i] != NULL) {
			mh->mh_element_free(mh->elements_arr[i]);
			mh->elements_arr[i] = NULL;
		}
	}
	if (mh->elements_arr != NULL) {
		free(mh->elements_arr);
		mh->elements_arr = NULL;
	}
	free(mh);
	mh = NULL;
	return success;
}

status insertToHeap(MaxHeap mh, element e) {
	if (mh == NULL || e == NULL) return null_pointer_failure;
	int current_size = mh->current_size;
	int max_size = mh->max_size;
	if (current_size == max_size) return add_failure;
	element temp_copy = mh->mh_element_copy(e);
	mh->elements_arr[mh->current_size] = temp_copy;
	if (mh->elements_arr[mh->current_size] == NULL) return memory_failure;
	mh->current_size++;
	HeapIncreaseLastElement(mh);
	return success;
}

status printHeap(MaxHeap mh) {
	status condition;
	if (mh == NULL) return null_pointer_failure;
	if (mh->heap_name == NULL) return print_failure;
	printf("%s:\n", mh->heap_name);
	if (mh->current_size == 0) printf("No elements.\n\n");
	else {
		element temp[mh->current_size];
		int i, original_amount = mh->current_size;
		for (i = 1; i <= original_amount; i++) {
			printf("%d. ", i);
			temp[i - 1] = PopMaxHeap(mh);
			mh->mh_element_print(temp[i - 1]);
		}
		for (i = 0; i < original_amount; i++) {
			condition = insertToHeap(mh, temp[i]);
			mh->mh_element_free(temp[i]);
		}
	}
	return condition;
}

element PopMaxHeap(MaxHeap mh) {
	if (mh == NULL) return NULL;
	if (mh->current_size == 0) return NULL;
	element top = mh->elements_arr[0];
	mh->elements_arr[0] = mh->elements_arr[mh->current_size - 1];
	mh->elements_arr[mh->current_size - 1] = NULL;
	mh->current_size--;
	MaxHeapify(mh, 0);
	return top;
}

element TopMaxHeap(MaxHeap mh) {
	if (mh == NULL) return NULL;
	if (mh->current_size == 0) return NULL;
	return mh->mh_element_copy(mh->elements_arr[0]);
}

char* getHeapId(MaxHeap mh) {
	if (mh == NULL) return NULL;
	return mh->heap_name;
}

int getHeapCurrentSize(MaxHeap mh) {
	if (mh == NULL) return -1;
	return mh->current_size;
}

int getHeapMaxCapacity(MaxHeap mh) {
	if (mh == NULL) return -1;
	return mh->max_size;
}

copyFunction getHeapCopyFunction(MaxHeap mh) {
	if (mh == NULL) return NULL;
	return mh->mh_element_copy;
}

freeFunction getHeapFreeFunction(MaxHeap mh) {
	if (mh == NULL) return NULL;
	return mh->mh_element_free;
}

printFunction getHeapPrintFunction(MaxHeap mh) {
	if (mh == NULL) return NULL;
	return mh->mh_element_print;
}

equalFunction getHeapEqualFunction(MaxHeap mh) {
	if (mh == NULL) return NULL;
	return mh->mh_elements_equal;
}
