#include<string.h>
#include<stdlib.h>
#include<stdio.h>
int main(int argc, char *argv[]) {
char *s;
void *ptr = NULL;

s = strdup("Tryme");  //should call malloc() implicitly 
puts(s);
free(s);
malloc(6);
malloc(0);
malloc(100);
malloc(1000);
free(malloc(10000));
free(malloc(0));

ptr = malloc(100);
realloc(ptr, 100);
realloc(malloc(100), 10);

ptr = malloc(100);
ptr = realloc(ptr, 1000);

return 0;
}
