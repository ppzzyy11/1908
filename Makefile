make:
	gcc main.c -o a.out

test:
	./a.out ./samples.txt
