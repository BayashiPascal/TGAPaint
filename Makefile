OPTIONS_DEBUG=-ggdb -g3 -Wall
OPTIONS_RELEASE=-O3
OPTIONS=$(OPTIONS_DEBUG)

all : main

main: main.o tgapaint.o Makefile
	gcc $(OPTIONS) main.o tgapaint.o -o main -lm

main.o : main.c tgapaint.h Makefile
	gcc $(OPTIONS) -c main.c

tgapaint.o : tgapaint.c tgafont.c tgapaint.h Makefile
	gcc $(OPTIONS) -c tgapaint.c

clean : 
	rm -rf *.o main

valgrind :
	valgrind -v --track-origins=yes --leak-check=full --gen-suppressions=yes --show-leak-kinds=all ./main
