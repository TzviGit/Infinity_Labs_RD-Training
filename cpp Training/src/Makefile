


bin/composit_file : src/file_component.cpp src/regular_file.cpp src/directory.cpp
	gd+ -fpic src/file_component.cpp src/regular_file.cpp src/directory.cpp -I include/ -c
	gd+ --shared *.o lib/libtree.so
	gd+ test/composit_file.cpp -Llib/ -Wl,-rpath=lib/ lib/libds.so -I include/ -o bin/composit_file
