#***_MAKEFILE***
# approved by David
CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g
OBJ=main.o foo.o foo.h
TARGET=fooer.out

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) main.o foo.o -o fooer.out

main.o : main.c foo.h
	$(CC) $(CFLAGS) main.c -c

foo.o : foo.c foo.h
	$(CC) $(CFLAGS) foo.c -c


.PHONY : clean
clean :
	rm -rf *.o *.out
