#include <stdio.h>
#include <stdint.h>

uint32_t b[100000000];

int main(int argc, char *argv[]){
	int i;
	b[0] = 1;
	
	for(i=0; i<50000000; i++){
		b[(2*i)+1] = b[i];
		b[(2*i)+2] = b[i] + b[i+1];
	}

	for(i=0; i<100000000; i++){
		printf("%u\n", b[i]);
	}
	return 0;
}
