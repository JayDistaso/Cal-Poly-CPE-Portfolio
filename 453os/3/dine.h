#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>

#define NUM_PHILOSOPHERS 3
#define CHANGING 2
#define THINKING 1
#define EATING 0

typedef struct Philosopher{
   /*ID of this phil*/
   int id;

   /*Thread coorosponding to this phil*/
   pthread_t thread;
   
   /*ID of fork this phil uses in left hand*/
   int leftForkID;
   
   /*ID of fork this phil uses in right hand*/
   int rightForkID;
   
   /* Current state of phil
   *  0 = Eating
   *  1 = Thinking
   *  2 = changing
   */ 
   int state;
   
} Philosopher;

typedef struct Fork{
   /*ID of this fork*/
   int id; 

   /*Pointer to phil holding this fork, or NULL if its on table*/
   Philosopher *owner;

   /*mutex lock coorpsonding to this fork*/
   pthread_mutex_t mutex_lock;
} Fork;

/*Table holding info on phils*/
Philosopher phils[NUM_PHILOSOPHERS];

/*Table holding info on forks*/
Fork forks[NUM_PHILOSOPHERS];

/*Mutex lock used when outputting state*/
pthread_mutex_t print_mutex_lock;

/*Default 1, optinally set my command line argument*/
int totalCycles = 1; 

/*Helper Functions*/

/*Prints header*/
void printHeader();

/*Prints Footer*/
void printFooter();

/*Prints status of each phil*/
void printState();

/*The thread main function, runs through phil actions*/
void *party(void *arg);

/*Picks up fork*/
void pickUpFork(Philosopher *philosopher, Fork *fork);

/*Puts down fork*/
void putDownFork(Philosopher *philosopher, Fork *fork);

/*sleeps for random amount of time*/
void dawdle();
