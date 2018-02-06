#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void main(){

	int PN;

	printf("Enter a prime number:");
	scanf("%d", &PN);


	for (int i = 2; i < PN; i++)
	{
		int num = i;
		for (int j = 1; j < PN; j++)
		{
			if (num == 1 && j != PN-1)
			{
					j = PN;
					break;
			}
			else if (num == 1 && j == PN-1)
			{
				printf("%d is a generator of Z_%d.", i, PN);
				i = PN;
				break;
			}
			else
			{
				if ((num*i) < PN)
				{
					num = (num*i);
				}
				else
				{
					int temp = (num*i)%PN;
					num = temp;
				}
			}
		}
	}
}

			
			
