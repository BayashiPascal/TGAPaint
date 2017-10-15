OPTIONS_DEBUG=-ggdb -g3 -Wall
OPTIONS_RELEASE=-O3
OPTIONS=$(OPTIONS_RELEASE)
INCPATH=/home/bayashi/Coding/Include
LIBPATH=/home/bayashi/Coding/Include

all : main

main: main.o tgapaint.o Makefile $(LIBPATH)/bcurve.o $(LIBPATH)/pbmath.o $(LIBPATH)/gset.o
	gcc $(OPTIONS) main.o tgapaint.o  $(LIBPATH)/pbmath.o $(LIBPATH)/bcurve.o $(LIBPATH)/gset.o -o main -lm

main.o : main.c tgapaint.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c main.c

tgapaint.o : tgapaint.c tgafont.c tgapaint.h $(INCPATH)/bcurve.h $(INCPATH)/gset.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c tgapaint.c

clean : 
	rm -rf *.o main

valgrind :
	valgrind -v --track-origins=yes --leak-check=full --gen-suppressions=yes --show-leak-kinds=all ./main

install :
	cp tgapaint.h ../Include; cp tgapaint.o ../Include
	
