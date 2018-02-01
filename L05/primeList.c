#include<stdlib.h>
#include<stdio.h>
#include<math.h>


void main() {
	
	int N;

	printf("Enter an upper bound:");
	scanf("%d", &N);

	int *isPrime = (int*) malloc(N*sizeof(int));

	for (int n=0;n<N;n++) isPrime[n] = 1;

	int sqrtN = (int) sqrt(N);

	for (int i=2; i<sqrtN; i++) {
		if (isPrime[i]) {
			for (int j=i*i; j<N;j+=i) {
				isPrime[j] = 0;
			}
		}
	}

	int cnt = 0;
	for (int n=0;n<N;n++) {
		if (isPrime[n]) {
			cnt++;
		}
	}

	int *primes = (int*) malloc(cnt*sizeof(int));

	cnt = 0;
	for (int n=0;n<N;n++) {
		if (isPrime[n]) {
			primes[cnt++] = n;
		}
	}

	for (int n=0;n<cnt;n++) printf("The %d-the prime is %d\n", n, primes[n]);

free(isPrime);
free(primes);

}

