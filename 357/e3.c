#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
   int data;
   struct Node *next; 
} Node;

void ReadList(Node **pHead) {
   int data;
   Node *nd, *temp;  // You may use temp, but don't have to.
   
   

printf("start read list\n");   
scanf("%d", &data);              
*pHead = NULL;                   
while (data != 0) {              
   nd = calloc(1, sizeof(Node)); 
   nd->data = data;              
   nd->next = NULL;              
   if (*pHead == NULL) {         
      *pHead = nd;               
   }                             
   else {                        
      temp->next = nd;           
   }                             
   temp = nd;                    
   scanf("%d", &data);           
}                                


}

void PrintList(Node *head) {
   while (head) {
      printf("%d ", head->data);
      head = head->next;
   }
   printf("\n");
}

Node **GetFinalPtr(Node **pHead) {
   
printf("start value of phead in getfinal is %p\n", pHead);
while ((*pHead)->next) {
   printf("node with data %d is phead\n", (*pHead)->data);            
   pHead = &((*pHead)->next); 
}
printf("final value of phead in getfinal is %p\n", pHead);
                           
return pHead;               


}

void ConcatLists(Node **pHead1, Node **pHead2) {
   

(*(GetFinalPtr(pHead1)))->next = *pHead2; 
*pHead2 = NULL;                         


}     

int main() {
   Node *head1, *head2;

   printf("Reading Lists...\n");
   ReadList(&head1);
   ReadList(&head2);
      printf("address of head1 and head2 targets= %p, %p\n", head1, head2);

   printf("Concatenating Lists...\n");
   ConcatLists(&head1, &head2);
   printf("Printing Lists...\n");
   printf("address of head1 and head2 targets= %p, %p\n", head1, head2);
   PrintList(head1);
   PrintList(head2);

   return 0;
}