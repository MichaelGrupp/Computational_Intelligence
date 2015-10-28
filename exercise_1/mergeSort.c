/*
Your program needs to read a single line of ASCII characters. The characters are constrained to 'A' - 'Z' only. The length of the line is unknown,
but well less than 100 characters. The line is terminated by '\n'. Sort all individual characters using the merge sort algorithm.
Remember that this algorithm uses a "divide&conquer" approach, by which it repeatedly breaks the sequence of characters into smaller portions before
sorting (left half and right half). For your convenience, in this problem the number of input characters is a power of 2 (so you can repeatedly divide your input in halves if required).
Use recursion to implement merge sort; i.e. make your own "sort" function call itself recursively with smaller parts of the data. Sort the "left half" first.
Whenever one instance of a recursion finishes print the result of this recursion only, followed by '\n'. Print the final sorted list exactly once at the end of your program.
Hint: Have your main program call your sort function with the full character sequence to start the recursion; this call will typically also print the final solution.
*/

#include<stdlib.h>
#include<stdio.h>

#define maxLen 102//let \n fit in

//print a subarray defined by left and right border indices
void printArray(char A[], int leftIndex, int rightIndex)
{
	int i;
	for (i = leftIndex; i <= rightIndex; i++)
		printf("%c", A[i]);
	printf("\n");
}

//merge the two parts charSet[leftIndex..midIndex] (left) and arr[midIndex+1..rightIndex] (right) of array charSet[]
void merge(char charSet[], int leftIndex, int midIndex, int rightIndex)
{
	int i, j, k;
	int leftSize = midIndex - leftIndex + 1;
	int rightSize = rightIndex - midIndex;

	//allocate temporary arrays
	char* left = malloc(leftSize*sizeof(char)); 
	char* right = malloc(rightSize*sizeof(char));

	//copy data to temporary arrays
	for (i = 0; i < leftSize; i++)
		left[i] = charSet[leftIndex + i];
	for (j = 0; j < rightSize; j++)
		right[j] = charSet[midIndex + 1 + j];


	//merge left and right
	i = 0;
	j = 0;
	k = leftIndex;
	while (i < leftSize && j < rightSize)
	{
		if (left[i] <= right[j])
		{
			charSet[k] = left[i];
			i++;
		}
		else
		{
			charSet[k] = right[j];
			j++;
		}
		k++;
	}

	//copy remaining elements of left[]
	while (i < leftSize)
	{
		charSet[k] = left[i];
		i++;
		k++;
	}

	//copy remaining elements of right
	while (j < rightSize)
	{
		charSet[k] = right[j];
		j++;
		k++;
	}
	printArray(charSet, leftIndex, rightIndex);
	free(right); free(left);
}

//merge sort a subarray of charSet defined by left and right border indices
void mergeSort(char charSet[], int leftIndex, int rightIndex)
{
	if (leftIndex < rightIndex)
	{
		int m = leftIndex + (rightIndex - leftIndex) / 2;
		mergeSort(charSet, leftIndex, m);
		mergeSort(charSet, m + 1, rightIndex);
		merge(charSet, leftIndex, m, rightIndex);
	}
}


int main()
{
	char charSet[maxLen];
	int l;
	
	fgets(charSet, maxLen, stdin);
	l = strlen(charSet) - 1; //the real length, without \n

	if (l > 1)
		mergeSort(charSet, 0, l - 1);
	else //one letter case
		printArray(charSet, 0, l - 1);

	return 0;
}