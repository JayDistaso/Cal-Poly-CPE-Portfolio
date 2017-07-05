#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
   int data;
   struct Node *next; 
} Node;

void ReadList(Node **pHead) {
   int data;
   Node *nd, *temp;  // You may use temp, but don't have to.
//printf("start read list\n");   
scanf("%d", &data);              // Base: 6, Surcharge: 0
*pHead = NULL;              // Base: 4, Surcharge: 0
while (data != 0) {       // Base: 5, Surcharge: 9.2
   nd = calloc(1, sizeof(Node)); // Base: 7, Surcharge: 0
   nd->data = data;                // Base: 3, Surcharge: 0
   nd->next = NULL;                // Base: 3, Surcharge: 0
   if (*pHead == NULL) {         // Base: 6, Surcharge: 0
      temp = *pHead = nd;          // Base: 9, Surcharge: 0
   }                             
   else {                        // Base: 1, Surcharge: 0
      temp = temp->next = nd;      // Base: 8, Surcharge: 0
   }                             
   scanf("%d", &data);           // Base: 6, Surcharge: 0
}  

}

void PrintList(Node *head) {
   while (head) {
   printf("Im in the while!\n");
      printf("%d ", head->data);
      head = head->next;
   }
   printf("\n");
}

void ConcatLists(Node **head1, Node **head2) {
   Node *temp;
   temp = *head1;
temp = *head1;        // Base: 4, Surcharge: 0
if (temp) {              // Base: 3, Surcharge: 0
   while (temp->next) {  // Base: 3, Surcharge: 0.8
      temp = temp->next; // Base: 3, Surcharge: 0
   }                     
   temp->next = *head2;  // Base: 4, Surcharge: 0
}                        
else {                   // Base: 1, Surcharge: 0
   *head1 = *head2;        // Base: 5, Surcharge: 0
}                        
*head2 = NULL;           // Base: 4, Surcharge: 0

}	  

int main() {
   Node *head1, *head2;
   
   printf("Reading Lists...\n");
   ReadList(&head1);
   ReadList(&head2);
   printf("Concatenating Lists...\n");
   ConcatLists(&head1, &head2);
   printf("Printing Lists...\n");
   PrintList(head1);
   printf("printing list 2\n");
   PrintList(head2);

   return 0;
}
