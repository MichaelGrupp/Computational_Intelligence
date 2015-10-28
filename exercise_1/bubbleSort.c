/* Your program needs to read a single line of ASCII characters. The characters are constrained to 'A' - 'Z' only.
The length of the line is unknown, but well less than 100 characters. The line is terminated by '\n'.
Sort all individual characters using the bubble sort algorithm.
After every complete pass through the data print the intermediate result terminated by '\n' (i.e. the partially sorted list).
Print the final sorted list exactly once at the end of your program.
Hint: you can use n=scanf("%s\n", s); l=strlen(s);  (where s is declared as char s[100]; and n, l as int n,l;)
to read the whole line at once and afterwards determine the number of characters read for this line.
*/

#include<stdio.h>

#define maxLen 100

int main() {
	char charSet[maxLen]; //the input that is going to be sorted
	char swap;
	int n, l;
	int swapped = 0; //C has no bool...

	fgets(charSet, maxLen, stdin);
	l = strlen(charSet) - 1; //the real length, without \n


	//swap loop
	do {
		swapped = 0;
		for (int j = 0; j < l - 1; j++) {
			if (charSet[j] > charSet[j + 1]) // decreasing order: <
			{
				swap = charSet[j];
				charSet[j] = charSet[j + 1];
				charSet[j + 1] = swap;
				printf("%s", charSet); //print with \n
				swapped = 1;
			}
		}
	} while (swapped == 1);

	return(0);
}