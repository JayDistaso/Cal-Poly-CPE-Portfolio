all: minget.c minls.c minlsget.h
	gcc -o minls minls.c
	gcc -o minget minget.c

minls: minls.c minlsget.h
	gcc -o minls minls.c

minget: minget.c minlsget.h
	gcc -o minget minget.c
