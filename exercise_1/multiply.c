#include<stdio.h>

int main() {
	int n = 0;
	int eof = 0;
	while (eof != -1) {
		eof = scanf("%d\n", &n);
		if (eof != -1)
			printf("%d\n", n * 2);
	}
	return(0);
}