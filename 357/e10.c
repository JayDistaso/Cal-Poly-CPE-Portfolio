#include <stdlib.h>
#include <stdio.h>
int main() {
	printf("char:%d\n", sizeof(char));
	printf("short:%d\n", sizeof(short));
	printf("int:%d\n", sizeof(int));
	printf("int *:%d\n", sizeof(int *));
	printf("char *:%d\n", sizeof(char *));
	printf("void *:%d\n", sizeof(void *));
}
