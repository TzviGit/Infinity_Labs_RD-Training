INCL = include/
SRC = src/
TST = test/
OBJ = obj/
BIN = bin/
LIB = lib/


CC = gcc
DBUG_FLAGS = -g

CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fPIC $(DBUG_FLAGS) $(DEF) -I $(INCL)
LDFLAGS = --shared

LPATH = $(LIB)
RPATH = -L$(LPATH) -Wl,-rpath=$(LPATH)
PRODUCT = ds
LIBRARY = lib$(PRODUCT).so

DEF=
SOURCES = $(wildcard $(SRC)*.c)
OBJECTS = $(patsubst $(SRC)%.c, $(OBJ)%.o, $(SOURCES))
BINARIES = $(patsubst $(SRC)%.c, %, $(SOURCES))



$(TARGET) : $(LIB)$(LIBRARY) $(TST)$(TARGET)_test.o
	mkdir -p bin test lib obj
	$(CC) $(CFLAGS) $(RPATH) $(TST)$(TARGET)_test.o -o $(BIN)$@ -l$(PRODUCT) -lm - lpthread

$(TST)%_test.o : $(TST)%_test.c $(INCL)%.h
	mkdir -p bin test lib obj
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB)$(LIBRARY) : $(OBJECTS)
	mkdir -p bin test lib obj
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ)%.o : $(SRC)%.c $(INCL)%.h
	mkdir -p bin lib obj
	$(CC) $(CFLAGS) -c $< -o $@

% : $(LIB)$(LIBRARY) $(TST)%_test.o
	mkdir -p bin lib obj
	$(CC) $(CFLAGS) $(RPATH) $^ -o $(BIN)$@ -l$(PRODUCT) -lm -lpthread


all :
	make files DBUG_FLAGS='-g'
	make files DBUG_FLAGS='-DNDEBUG -O3' PRODUCT='ds_release'

debug :
	make clean
	make files DBUG_FLAGS='-g'

release :
	make clean
	make files DBUG_FLAGS='-DNDEBUG -O3' PRODUCT='ds_release'


tests :
	make binaries DBUG_FLAGS='-g'

files : $(LIB)$(LIBRARY)

binaries : $(BINARIES)


.PHONY : clean
clean :
	rm -f $(OBJ)*.o $(TST)*.o $(BIN)*

.PHONY : clean_tests
clean_tests :
	rm -f $(TST)*.o
