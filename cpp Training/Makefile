
CC = g++ -ansi -pedantic-errors -Wall -Wextra -g


bin/composit_file : src/file_component.cpp src/regular_file.cpp src/directory.cpp test/composit_file_test.cpp
	$(CC) -fpic src/file_component.cpp src/regular_file.cpp src/directory.cpp -I include/ -c
	$(CC) --shared *.o -o lib/libtree.so
	$(CC) test/composit_file_test.cpp -Llib/ -Wl,-rpath=lib/ -ltree -I include/ -o bin/composit_file
	rm *.o

clean:
	rm bin/composit_file

	