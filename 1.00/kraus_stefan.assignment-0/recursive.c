#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t calculate(uint32_t n){
	if (n == 1 || n == 0){
		return 1;
	}
	else if (n%2 == 0){
		return (calculate((n/2)-1) + calculate(n/2));
	}
	else {
		return calculate((n-1)/2);
	}
}

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Improper Use!");
		return -1;
	}
	uint32_t n= atoi(argv[1]);
	printf("%u\n", calculate(n));
	return 0;
}
