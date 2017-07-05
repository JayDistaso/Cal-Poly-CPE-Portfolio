#include <stdio.h>

#define WORD_LEN 10
#define SENTENCE_LEN 100

void ConcatStr(char *trg, char *src) {
   while (*trg) {
      trg++;
   }
   while (*src) {
      *trg = *src;
      trg++;
      src++;
   }
   trg++;
   *trg = *src;
}

int main() {
   char word[WORD_LEN + 1];
   char sentence[SENTENCE_LEN + 1] = "";
   //
   while (EOF != scanf("%10s", word)) {
      ConcatStr(sentence, word);
      ConcatStr(sentence, " ");
   }

   printf("%s\n", sentence);
   return 0;
}
