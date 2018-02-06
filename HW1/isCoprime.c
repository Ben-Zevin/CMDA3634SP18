#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void main(){

	int N1;
	int N2;
	int gcd;
	int temp;

	printf("Enter the first number:");
	scanf("%d", &N1);
	printf("Enter the second number:");
	scanf("%d", &N2);

	for (int i = 1; i <= N1 && i <= N2; i++)
	{
			if (N1%i == 0 && N2%i == 0)
			{
					gcd = i;
			}
	}

	if (gcd == 1)
	{
			printf("%d and %d are coprime.", N1, N2);
	}
	else
	{
			printf("%d and %d are not coprime.", N1, N2);
	}
}


