#include <stdio.h>
#include <string.h>

/*
 * Initially used a strack to as an extra data structure then I came across an observationa and wrote this.
 * */
void reverseMe(char str[], int len) {
	printf("reversing: %s \n", str);

	// swap the opposites as you move inside.
	int l = 0;

	int limit = (len - 1)/2;
	
	int r;

	char temp;
	while (l <= limit) {
		r = len - 1 - l;
		temp = str[l];
		str[l] = str[r];
		str[r] = temp;
		l++;
	};

	printf("reversed: %s \n", str);
}

int main() {
    char tests[5][2][10] = {
        {"", ""},
        {"k", "k"},
        {"foo", "oof"},
        {"atom", "mota"},
    };

    for (int i = 0; i < 4; i++) {
        char* test[2] = {tests[i][0], tests[i][1]};
        char* given = test[0];
        char* expect = test[1];

        reverseMe(given, strlen(given));

        if (strcmp(given, expect) != 0) {
            printf("Failed Test: Expected %s got %s \n", expect, given);
        }
    }

    return 0;
}
