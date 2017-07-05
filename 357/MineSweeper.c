/*
*  CPE 357
*  Project 1: Minesweeper
*  Justin Distaso
*/

#include <stdio.h> //main library
#include <string.h>
#include <stdlib.h>
#define FIELD_SIZE 20
#define max(a,b) (a>b?a:b)

void printBoard();
void printfinalBoard();
void updateBoard(int row, int col);
int checkwin();
void displayEOF();

char field[FIELD_SIZE+2][FIELD_SIZE+2];//Holds the current playing field and bigger for easier checking
char finalfield[FIELD_SIZE+2][FIELD_SIZE+2];//Bigger to hold edges for easier checking

int main(int argc, char *argv[]) {//change this later
   
   //Declare/Initialize things
   int row, col;
   int mines[FIELD_SIZE][FIELD_SIZE];//Holds where mines are bigger for easier checking
   int i, j;
   int mineCount = 0;
   int minesAdded;
   int minesNearby;
   int k, kk;
   
   //Fill field with *'s
   for (i = 0; i < FIELD_SIZE+2; i++) {
      for (j = 0; j < FIELD_SIZE+2; j++) {
           field[i][j] = '*';
      }
   }
   
   //Put in location of mines
   //Something about EOF and minesadded
   if(scanf("%d", &mineCount) == EOF){
         displayEOF();
   }
   if(mineCount < 0){
      printf("Number of mines cannot be negative\n");
      exit(-1);
   }
   
   for(; minesAdded < mineCount; minesAdded++){
      scanf("%d", &row);
      scanf("%d", &col);
      if(row > 19 || row < 0 || col > 19 || col < 0){
         printf("(%d, %d) is not a valid position for a mine.", row, col);
      }
      mines[row][col] = 1;
   }
   //Calculate board
   //Put all placeholders in board (might not be necessary)
   for (i = 0; i < FIELD_SIZE+2; i++) {
      for (j = 0; j < FIELD_SIZE+2; j++) {
           finalfield[i][j] = '*';
      }
   }
   
   //Place mines
   for (i = 0; i < FIELD_SIZE; i++) {
      for (j = 0; j < FIELD_SIZE; j++) {
           if(mines[i][j] == 1){
               finalfield[i+1][j+1] = 'X';
           } 
      }
   }
   
   //Calculate Numbers
   for (i = 1; i < FIELD_SIZE+1; i++) {
      for (j = 1; j < FIELD_SIZE+1; j++) {
           minesNearby = 0;
           for(k = 0; k < 3; k++){
               for(kk = 0; k < 3; k++){
                  row = i-1+k;
                  col = j-1+kk;
                  if(finalfield[row][col] == 1){
                     minesNearby++;
                  }
               }
           }
           if(finalfield[i][j] != 'X'){//Don't put a number where the mines are
             finalfield[i][j] = minesNearby;
           }
      }
   }
   
   //Print initial board
   int round = 0;
   int flag = 1;
   while(flag == 1)//Keep the game going
   {
      if(scanf("%d", &row) == EOF){
         displayEOF();
      }
      if(scanf("%d", &col) == EOF){
         displayEOF();
      }
      if(row > 19 || row < 0 || col > 19 || col < 0){
         printf("(%d, %d) is not a valid position.", row, col);
      }
      //Add one for offset board
      row++;
      col++;
      
      if(field[row][col] != '*'){
         printf("Spot already revealed, pick another spot");
      }
      else if(finalfield[row][col] == 1){
        printf("You Lose...");
        printBoard();
      }
      
      else{
         updateBoard(row, col);
         flag = checkwin();
         printf("Round: %d\n", round);
         printBoard();
         round++; 
      }
   }
   printf("You Win!!!\n");
   printfinalBoard();
   return 0;
}

//Function to print board
void printBoard(){
   int i,j;
   for (i = 1; i < FIELD_SIZE+1; i++) {
   printf("\n");
      for (j = 1; j < FIELD_SIZE+1; j++) {
           printf("%c", field[i][j]);
           printf(" ");
      }
   }
}
void printfinalBoard(){
   int i,j;
   for (i = 1; i < FIELD_SIZE+1; i++) {
   printf("\n");
      for (j = 1; j < FIELD_SIZE+1; j++) {
           printf("%c", finalfield[i][j]);
           printf(" ");
      }
   }
}

void updateBoard(int row, int col){
   int i, j, k, kk;
   field[row][col] = finalfield[row][col];
   if(finalfield[row][col] != '0'){ //Nothing else to do
   }
   else{//reveal zeros
      int reveal = 1;
      while(reveal == 1){ 
         reveal = 0; 
         for (i = 1; i < FIELD_SIZE+1; i++) {
            for (j = 1; j < FIELD_SIZE+1; j++) {
               for(k = 0; k < 3; k++){
                  for(kk = 0; k < 3; k++){
                     row = i-1+k;
                     col = j-1+kk;
                     if(field[row][col] == '0' && finalfield[i][j] == '0' && field[i][j] != '0'){
                        field[i][j] = '0';
                        reveal = 1;
                     }
                  }
               }
            }
         }
      }   
   }
}
int checkwin(){
   int i, j, win = 0;
   for (i = 1; i < FIELD_SIZE+1; i++) {
      for (j = 1; j < FIELD_SIZE+1; j++) {
           if(field[i][j] == '*'){
               win = 1;
           }
      }
   }
   return win;
}

void displayEOF(){
   printf("No more input. Game Over\n");
   exit(-1); 
}

