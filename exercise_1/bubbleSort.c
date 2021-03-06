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
	char charSet[maxLen];
	char swap;
	int l;
	int swapped = 1, alreadySorted = 1; //C has no bool...

	fgets(charSet, maxLen, stdin);
	l = strlen(charSet) - 1; //the real length, without \n

	while (swapped == 1) {
		swapped = 0;
		//swap loop
		for (int j = 0; j < l - 1; j++) {
			if (charSet[j] > charSet[j+1]) // decreasing order: <
			{
				swap = charSet[j];
				charSet[j] = charSet[j+1];
				charSet[j + 1] = swap;
				swapped = 1;
			}
		}
		//print every intermediate result
		if (swapped) {
			printf("%s", charSet); //print with \n
			alreadySorted = 0;
		}
	}

	//print exactly once, even if already sorted
	if(alreadySorted)
		printf("%s", charSet);

	return(0);
}