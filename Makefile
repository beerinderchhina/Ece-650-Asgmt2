all: Graph.c
	gcc -Wall -std=c89 Graph.c -o a2-ece650

clean:
	rm -f a2-ece650
