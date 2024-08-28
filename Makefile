all: main.c main.h
	rm -f dns
	gcc main.c -o dns.out