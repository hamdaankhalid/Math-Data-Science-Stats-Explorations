#include <stdio.h>

void printBinary(void* data, size_t size) {
    unsigned char* ptr = (unsigned char*)data;
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            unsigned char bit = (ptr[i] >> j) & 1;
            printf("%u", bit);
        }
        printf(" ");
    }
    printf("\n");
}

int main() {
	// memory layout according to virtual memory
	// show the smallest size in memory that you can address
	char smallest = 'c';
	printf("A character which is the smallest data type you can make is of %zu bytes \n", sizeof(smallest));
	printf("%c is represented as: ", smallest);	
	printBinary(&smallest, sizeof(smallest));

	printf("The data assigned in the above example is of %zu byte(s) and is stored at the following virtual memory address: %p\n", sizeof(smallest), &smallest);

	printf("That weird address you see above is called hexadecimal and it is just a more human readable form of binary\n");

	// what does an unsigned integer look like in binary
	int signedInt = -3;
	unsigned int unsignedInt = 3;
	
	printf("%d is of size %zu byte(s) represented as: ", unsignedInt, sizeof(unsignedInt));
	printBinary(&unsignedInt, sizeof(unsignedInt));

	printf("%d is of size %zu byte(s) represented as: ", signedInt, sizeof(signedInt));
	printBinary(&signedInt, sizeof(signedInt));
	// both 4 bytes ^ but their representation is different because of 2's complement

	// show what a longer number would look like

	// what does a floating number look like
		

	// what does a string look like
	// what does an array look like
	// what does a struct look like
	// how are types maintained?
	// what does a hashmap look like
	// what does a set look like
	// what does a linked list look like
	// what does a doubly linked list look like
	return 0;
}
