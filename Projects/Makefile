FULL_PATH = /home/motzvi/git/
PR_PATH = $(FULL_PATH)projects/
DS_PATH = $(FULL_PATH)ds/

INC = include/
SRC = src/
TST = test/
OBJ = obj/
OBJ_R = obj_r/
BIN = bin/
LIB = lib/

CC = gcc
DBUG = -g
DEF =
THRD = -pthread
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fPIC $(DBUG) $(DEF) $(THRD)
LDFLAGS = -shared
LMFLAGS = -lm

DS_LPATH = '$(DS_PATH)$(LIB)'
DS_RPATH = -L$(DS_LPATH) -Wl,-rpath=$(DS_LPATH)
DS_PRODUCT = ds
DS_LIBRARY = lib$(DS_PRODUCT).so

LPATH = '$(PR_PATH)$(LIB)'
RPATH = -L$(LPATH) -Wl,-rpath=$(LPATH)
PRODUCT = pr
PRODUCT_R = pr_r
LIBRARY = lib$(PRODUCT).so

DS_SOURCES = $(wildcard $(DS_PATH)$(SRC)*.c)
DS_OBJECTS = $(patsubst $(DS_PATH)$(SRC)%.c, $(DS_PATH)$(OBJ)%.o, $(DS_SOURCES))
SOURCES = $(wildcard $(SRC)*.c)
OBJECTS = $(patsubst $(SRC)%.c, $(OBJ)%.o, $(SOURCES))
BINARIS = $(patsubst $(SRC)%.c, %, $(SOURCES))


# TEST
$(TST)%_test.o : $(TST)%_test.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC) -I $(DS_PATH)$(INC)

# LIBRARY
$(LIB)$(LIBRARY) : $(OBJECTS) $(DS_OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LMFLAGS) -o $@

# OBJECT
$(OBJ)%.o : $(SRC)%.c $(INC)%.h
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC) -I $(DS_PATH)$(INC)

# OBJECT - DS
$(DS_PATH)$(OBJ)%.o : $(DS_PATH)$(SRC)%.c $(DS_PATH)$(INC)%.h
	$(CC) $(CFLAGS) -c $< -o $@ -I $(DS_PATH)$(INC)

# EXE
% : $(LIB)$(LIBRARY) $(TST)%_test.o
	$(CC) $(CFLAGS) $(RPATH) $^ -o $(BIN)$@ -l$(PRODUCT) $(LMFLAGS)



.PHONY : dirs, all, debug, release, tests, release_tests, files, binaries

dirs :
	mkdir -p $(BIN) $(LIB) $(OBJ) $(OBJ_R)

all :
	make debug
	make release

debug :
	make dirs
	make files

release :
	make dirs
	make files DBUG='-DNDEBUG -O3' PRODUCT=$(PRODUCT_R) OBJ=$(OBJ_R)

ds :
	make $(BIN)wd

ds_files :
	$(DS_PATH)$(LIB)$(DS_LIBRARY)

tests :
	make dirs
	make binaries

release_tests :
	mkdir -p $(BIN) $(LIB) $(OBJ)
	make binaries DBUG='-DNDEBUG -O3' PRODUCT=$(PRODUCT_R) OBJ=$(OBJ_R)

files : $(LIB)$(LIBRARY)

binaries : $(BINARIS)


.PHONY : clean
clean :
	rm -f $(OBJ)*.o $(OBJ_R)*.o $(TST)*.o $(BIN)*

.PHONY : clean_tests
clean_tests :
	rm -f $(TST)*.o
