/* Your program needs to read a single line of ASCII characters. The characters are constrained to 'A' - 'Z'. 
The length of the line is unknown, but well less than 100 characters. The line is terminated by '\n'. 
Sort all individual characters using the insertion sort algorithm. 
Start sorting with the second element from the beginning of the line (as the first cannot get inserted anywhere to its left). 
After handling an element print the intermediate result terminated by '\n' (i.e. the partially sorted list). 
Note that the output from handling the last element represents the final sorted list; so no need to create an additional output.
*/


#include<stdio.h>

#define maxLen 102//let \n fit in

int main() {
	char charSet[maxLen];
	char keyVal;
	int l, j;
	int inserted = 1, alreadySorted = 1; //C has no bool...

	fgets(charSet, maxLen, stdin);
	l = strlen(charSet) - 1; //the real length, without \n

	for (int i = 1; i <= l - 1; i++) {
		inserted = 0;
		keyVal = charSet[i];
		j = i - 1;
		//search insertion point (backwards from keyVal)
		while (charSet[j] > keyVal && j >= 0) {
			charSet[j + 1] = charSet[j];
			j = j - 1;
			alreadySorted = 0;
		}
		//insert at insertion point
		charSet[j+1] = keyVal;
		printf("%s", charSet);
	}

	//print exactly once, even if already sorted
	if (alreadySorted)
		printf("%s", charSet);

	return(0);
}