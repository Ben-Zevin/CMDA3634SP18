#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void main(){

	int N;

	printf("Enter a number:");
	scanf("%d", &N);

	int sqrtN = (int) sqrt(N);
	int gd = 1;

	for (int i = 2; i<=sqrtN; i++)
	{
			if (N%i == 0)
			{
					gd = i;
			}
	}

	if (gd == 1)
	{
			printf("%d is prime.", N);
	}
	else
	{
			printf("%d is not prime.", N);
	}
}

