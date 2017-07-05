#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	int fd[2], bit = 0;
	long counter = 0;
	char *buf = malloc(1 * sizeof(char));
	//0 read
	// 1 write

//	printf("testing |%c|\n", buf);
//	printf("testing sizeof = %d\n", sizeof(buf));

	pipe(fd);

	while(bit != -1) {
		bit = write(fd[1], buf, 1);
		counter++;
		//printf("the bit was %d, and counter is %ld\n", bit, counter);
		if (counter == 65536) {
			printf("The Buffer is blocking so the max size must be %d\n", counter);
			return 0;
		}
	}
	printf("exited the while loop with counter = %ld\n", counter);
	return 0;
}
