all: Graphc.c
	gcc -Wall -std=c89 Graphc.c -o a2-ece650

clean:
	rm -f a2-ece650
