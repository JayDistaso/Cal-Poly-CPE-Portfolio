hello: hello.c
	gcc -Wall -ansi -pedantic -g -c -o hello.o hello.c
	gcc -g -o hello hello.o
test:
	gcc -Wall -ansi -pedantic -g -c -o hello.o hello.c
	gcc -g -o hello hello.o
	@echo "Testing hello..."
	@./hello
clean:
	rm -f hello.o	
