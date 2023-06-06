#include <stdio.h>
#include <string.h>

/*
 * Initially used a strack to as an extra data structure then I came across an observationa and wrote this.
 * */
void reverseMe(char str[], int len) {
	printf("reversing: %s \n", str);

	// swap the opposites as you move inside.
	int l = 0;

	int limit = len/2;
	
	int r;

	char temp;
	while (l < limit) {
		// can I somehow use SIMD here or something?
		r = len - 1 - l;
		temp = str[l];
		str[l] = str[r];
		str[r] = temp;
		l++;
	};

	printf("reversed: %s \n", str);
}

int main() {
	char test[] = "kak dela? Normalna";
	
	reverseMe(test, strlen(test));

	return 0;
}
