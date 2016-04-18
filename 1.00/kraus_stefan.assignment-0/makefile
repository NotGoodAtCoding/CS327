all: recursive iterative

recursive: recursive.c
	gcc -g -Wall -Werror recursive.c -o recursive

iterative: iterative.c
		gcc -g -Wall -Werror iterative.c -o iterative

clean:
	rm iterative
	rm recursive
