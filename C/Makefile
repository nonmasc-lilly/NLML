all: main.c
	gcc main.c -o nmc -ansi
test: all
	./nmc test.nlml testfile.txt output.txt
	xxd output.txt
	cat output.txt
install: all
	cp nmc /usr/local/bin/nmc
