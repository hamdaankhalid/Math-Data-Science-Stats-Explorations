#include <stdio.h>
#include <string.h>

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

struct Node {
	int value;
	struct Node* next;
};

int main() {
	
	printf("Computers understand binary. All our instructions and data is stored in bionary format. We have abstractions that take data structures like strings, and lists, and instructions "
			"and store them as binary. Today we are going to explore how this conversion happens, what your data structures look like under the hood and how we can write better "
			"programs when we understand the underlying memory layout. \n"
			"Memory layout according to virtual memory is a continuous list of byte (8 bit) chunks that is composed of physical memory "
			"from the RAM and Disk. Using a technique called paging, and caching the Operating System (OS) provides an abstraction that lets us use "
			"memory in the form of a list of addresses that we can place data in. This is known as virtual memory.\n "
			"Wait what's a byte? Is it the same as a bit? Nope! A byte is basically a group of 8 bits. 8 bits means each byte can represent 2^8 (256) "
			"unique values. I tried finding out why we settled on using 8 bits as the standard of smallest addressable memory and not say like 4. "
			"Turns out it was chosen as a tradeoff between how many values could be represented while keeping a balance of comple addressing schemes. "
			"It then eventually became a standard, some systems are built to work with even smaller addressable units like 'nibble' but byte is the standard. \n");
	
	// show the smallest size in memory that you can address
	char smallest = 'c';
	printf("A character which is the smallest data type you can make is of %zu bytes \n", sizeof(smallest));
	printf("%c is represented as: ", smallest);	
	printBinary(&smallest, sizeof(smallest));

	printf("The character can also be represented in hexadecimal as => 0x%x\n", smallest);
	printf("what we are basically doing is using binary numbers in different contexts to convey different meanings\n");
	printf("The above is known as encoding: Ascii encoding is the most widely used, and it works by assigning numeric values to individual characters\n");

	printf("The data assigned in the above example is of %zu byte(s) and is stored at the following virtual memory address: %p\n", sizeof(smallest), &smallest);

	printf("That weird address you see above is called hexadecimal and it is just a more human readable form of binary\n");

	// what does an unsigned integer look like in binary
	int signedInt = -3;

	unsigned int unsignedInt = 3;
	
	printf("%d is of size %zu byte(s) represented as: ", unsignedInt, sizeof(unsignedInt));
	printBinary(&unsignedInt, sizeof(unsignedInt));
	printf("Unsigned integers are represented as the standard binary, but the standard understanding of binary encoding ignores the representation of negative numbers. "
			"Computers can represent more than just postiive integers that fit in %zu byte(s). Well for that we have to expand our encoding of binary numbers and how we read them. "
			"We need a way to show number that are negative, numbers that are decimals, and number that need to be larger and smaller that %zu byte(s). \n", sizeof(unsignedInt), sizeof(unsignedInt));

	printf("The negative number: %d is of size %zu byte(s) represented as: ", signedInt, sizeof(signedInt));
	printBinary(&signedInt, sizeof(signedInt));

	printf("Do you notice something unusual? We flipped the sign from %d but the representation does not even look close to what the representation of %d looked like.\n", unsignedInt, unsignedInt);
	
	printf("This is known as complement of 2 encoding of binary numbers and can be used to form negative numbers!\n");

	float floaty = 5.3;
	printf("A floating point number is one of the ways we can store umbers that have a fractional part such as %.2f\n. They are represented as: \n", floaty);
	printBinary(&floaty, sizeof(floaty));
	printf("The most common way to encode these numbers with fractional parts is the IEEE floating-point format, \n");

	printf("Now certain data types that we need require using more bytes, these data types include things like 'double' and 'long', which dependending on the system will use more bytes");

	printf("Before we move to learning about the more fun data structures and their representation, we need to address Endianess. \n"
			"Endianness refers to which direction the binary number is laid out in memory. Big Endian the most significant bit is stored first "
			"followed by the more significant bits. In Little endian the layout is opposite.\n"
			"Endianess is important to address because when dealing with data being exchanghed across systems or interacting with external data, we need to know endianness to interpret it correctly.\n"
			"Most machines today support the little endian architecture by default on X86 and X86_64 architecture.\n");

	// what does a string look like
	const char* text = "this is a string";
	printf("Strings are represented as 'char*' in C that terminate with the null character '\\0'.\n"
			"The char* means that a string is a pointer to a character in memory. A string is hence 'composed' of characters that point to each other and finih with \\0. \n"
			"A pointer in C stores the memory address of a variable, we can address the value at that address by either 'dereferencing' a pointer by calling *pointer' or "
			"by using array style indexing. Array style indexing means if we have a string 'char* mystr = 'lalala' I can access the value at the first character of mystr "
			"via mystr[0] and get the character 'l', the next one via mystr[1] and so on.\n"
			"Now if I try to dereference something out of the bound, then I run into undefined behavior, the program may crash, there may be segmentation fault, or maybe nothing. \n"
			"The undefined behavior is not defined by C standard since we are accessing memory outside what was assigned for the string\n");
	
	printf("Here is a string we will be iterating over via memory address and dereferencing %s", text);
	for (size_t i = 0; i < strlen(text); i++) {
		printf("%c\n", text[i]);
	}
	printf("Notice how in the string we just know that the next character is present in the next byte! This is because memory allocated for a string is continuous chunk. \n"
			"This means that we can get to any part of our string in constant time as long as we know the offset adn the starting virtual address. \n"
			"This moving from the virtual address to a differnt byte in memory is known as pointer arithmetic, and we use it to iterate over the characters of our string, or "
			"reach different parts of our string. \n");
	
	// what does an array look like
	printf("Now we have concluded that strings are collections of characters in one continuous chunk of virtual memory, and we can reach any part of a string in constant time "
			"as long as we know the starting address and the offset we want to get to. We can now graduate to Arrays! Arrays are exactly like string except that instead of characters "
			"an array may be comprised of any type. \n");

	const int myarr[] = {10, 15, 4500};
	
	printf("Arrays are composed of 2 things, a type, and their size. The type and size/capacity of the array are used to figure out how much contiguous virtual memory space needs to be allocated.\n"
			"For an array of 3 integers, we would need memory = size of an int * 3 = 4*3 = 12 bytes.\n"
			"If we want to go from one element to another in the array we need to hop to the address 4 bytes at a time! This is unlike a string where we went to the block one byte ahead, this is because the elements in our array where only 1 byte each\n"
			"Luckily the C compiler scales our step appropriately based on the type which lets us iterate over and access items in our array without explicitly jumping by N bytes.\n"
			"Given the structure of an array in the virtual memory, things like random access and a concept called 'cache performance' really shine.\n");

	for (int i = 0; i < 3; i++) {
		printf("%d\n", *(myarr+i));
	}

	// what does a struct look like
	printf("With the primitves covered, and lists covered, there's only one thing we need to know before we can start looking into the data structures we build like "
			"linked lists, hashmaps, sets, and trees. We need to cover what a Struct / Structure looks like in memory!\n"
			"A struct is a user defined data type that is composed of multiple primitives, or other user defined data types. \n"
			"Using a struct we can create instances that follow the structs schema/structure. We could make a struct that holds an int along with a list of 3 characters. \n"
			"If we wanted to create a linked list we could create a node with an int that signifies a nodes value, and have a field that holds a pointer to another node in memory! \n"
			"We can literally build our own data types, and write methods that interact with these types to create a higher layer of abstraction, how exciting! \n"
			"Let's take a look into how these structs are stored in the memory first though. \n");

	// how are types maintained?
	// what does a hashmap look like
	// what does a set look like
	// what does a linked list look like
	// what does a doubly linked list look like
	return 0;
}
