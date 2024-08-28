all: main.c main.h
	rm -f dns.out
	gcc main.c -o dns.out