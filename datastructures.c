#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct ResizableArray {
	void** data;
	int capacity;
	int occupied;
	unsigned int sizeOfElement;
};

struct ResizableArray* newArray(unsigned int sizeOfElement) {
	struct ResizableArray* arr = malloc(sizeof(struct ResizableArray));
	if (!arr) {
		printf("Failed to allocate memory while create the array\n");
		return NULL;
	}
	arr->data = malloc(8 * sizeof(arr->sizeOfElement));
	if (!arr->data) {
		printf("Failed to allocate memory for internal array\n");
		free(arr);
		return NULL;
	}
	arr->sizeOfElement = sizeOfElement;
	arr->capacity = 8;
	arr->occupied = 0;
	return arr;
}

int addElement(struct ResizableArray* arr, void* element) {
	if (arr->occupied == arr->capacity) {
		// increase capacity by double
		arr->data = realloc(arr->data, arr->capacity * sizeof(arr->sizeOfElement) * 2);
		if (!arr->data) {
			printf("Failed to resize the array\n");
			return -1;
		}
		arr->capacity *= 2;
	}
	
	void* elementToSave = malloc(arr->sizeOfElement);
	if (elementToSave) {
		memcpy(elementToSave, element, arr->sizeOfElement);
		arr->data[arr->occupied] = elementToSave; 
		arr->occupied++;
		return 0;
	} else {
		printf("Error allocating memory when saving to resizable array\n");
		return -1;
	}
}

void removeElement(struct ResizableArray* arr) {
}

