#include "dine.h"

int main(int argc, char *argv[]) {
   pid_t ppid; /*For process ID*/
   int id; /*For loop counter and ID*/
   int retVal;
   //int returnValue = 0; /*Value returned by pthread operations*/
   
   /* The id of process */
   int ids[NUM_PHILOSOPHERS];
   
   /* Where the thread descriptor will go */
   //pthread_t childid[NUM_PHILOSOPHERS];
   
   /* initialize the parent process id for id: later use */
   ppid = getpid();
   
   for (id = 0; id < NUM_PHILOSOPHERS; id++) {
      ids[id] = id;
   }
   
   if (argc >= 2) {
      totalCycles = strtol(argv[1], NULL, 10);
   }
   
   /*Init Forks */
   for (id = 0; id < NUM_PHILOSOPHERS; id++) {
      forks[id].id = id;
      forks[id].owner = NULL;

      /*init mutex locks*/
      if ((retVal = pthread_mutex_init(&forks[id].mutex_lock, NULL)) != 0) {
         fprintf(stderr, "pthread_mutex_init failed for id: %d with error %d",
          id, retVal);
         exit(-1);
      }
   }
   
   /*Init the Philosophers*/
   for (id = 0; id < NUM_PHILOSOPHERS; id++) {
      phils[id].id = id;
      phils[id].leftForkID = ((id + 1) % NUM_PHILOSOPHERS);
      phils[id].rightForkID = (id % NUM_PHILOSOPHERS);
      phils[id].state = CHANGING;
   }
   
   printHeader();
   
   /*Seat the phils (create their pthreads*/
   for (id = 0; id < NUM_PHILOSOPHERS; id++) {      
      if (pthread_create(&phils[id].thread, NULL, party, (void *)(&ids[id]))) {
        fprintf(stderr, "pthread_create failed for id: %d", id);
        exit(-1); 
      }  
   }
   
   /*Wait for id: all phils to finish*/
   for (id = 0; id < NUM_PHILOSOPHERS; id++) {      
      if (pthread_join(phils[id].thread, NULL)) {
        fprintf(stderr, "pthread_join failed for id: %d", id);
        exit(-1); 
      }  
   }
   
   /*Destroy mutex locks*/
   for (id = 0; id < NUM_PHILOSOPHERS; id++) {
      if (pthread_mutex_destroy(&forks[id].mutex_lock)) {
         fprintf(stderr, "pthread_destroy failed for id: %d", id);
         exit(-1);    
      }
   }
   
   /*Destory global mutex*/
   if (pthread_mutex_destroy(&print_mutex_lock)) {
      fprintf(stderr, "pthread_destroy failed for id: %d", id);
      exit(-1); 
   }
   
   printFooter();
   
   return 0;
}

/* This is the function the pthreads get made
   into, the actual dinner party. Has to have void *arg
   as its paramater but its really where the id goes */
void *party(void *arg) {
  
   int cycle;
   Philosopher *phil = &phils[*(int *)arg];
   
   for (cycle = 0; cycle < totalCycles; cycle++) {
      /*Even tries right fork first, odd tries left fork first*/
      if (phil->id % 2 == 0) {
         pickUpFork(phil, &forks[phil->rightForkID]);
         pickUpFork(phil, &forks[phil->leftForkID]);
      }
      else {
         pickUpFork(phil, &forks[phil->leftForkID]);
         pickUpFork(phil, &forks[phil->rightForkID]);
      }
      
      /*If we made it here we phil has both forks, so he's eating*/
      phil->state = EATING;
      printState();
      
      /*dawdle as per spec*/
      dawdle();
      
      /*Done dawdling, transitioning*/
      phil->state = CHANGING;
      
      /*Order he puts down fork does not matter, but one at a time*/
      putDownFork(phil, &forks[phil->leftForkID]);
      putDownFork(phil, &forks[phil->rightForkID]);
      
      /*Forks down, ready to think*/
      phil->state = THINKING;
      dawdle();
      
      /*Done thinking, hungry again*/
      phil->state = CHANGING;
   }
   
   pthread_exit(NULL); 
}

void dawdle() {
   /*
    * sleep for a random amount of time between 0 and 999
    * milliseconds. This routine is somewhat unreliable, since it
    * doesn’t take into account the possiblity that the nanosleep
    * could be interrupted for some legitimate reason.
    *
    * nanosleep() is part of the realtime library and must be linked
    * with –lrt
    */
   struct timespec tv;
   int msec = (int)(((double)random() / INT_MAX) * 1000);
   
   tv.tv_sec = 0;
   tv.tv_nsec = 1000000 * msec;
   
   if (-1 == nanosleep(&tv,NULL)) {
      perror("nanosleep");
   }

}

void pickUpFork(Philosopher *phil, Fork *fork) {
   
   if (pthread_mutex_lock(&(fork->mutex_lock))) {
      perror("Fork Mutex Lock");
      exit(-1);
   }
   
   fork->owner = phil;
   printState();
   
}

void putDownFork(Philosopher *phil, Fork *fork) {
   
   fork->owner = NULL;
   printState();
   
   if (pthread_mutex_unlock(&(fork->mutex_lock))) {
      perror("Fork Mutex Unlock");
      exit(-1);
   }
   
}

void printState() {
   Philosopher *phil;
   Fork *fork;
   
   int philCount, forkCount;
   
   pthread_mutex_lock(&print_mutex_lock);
   
   /*For each phil we print its state*/
   for (philCount = 0; philCount < NUM_PHILOSOPHERS; philCount++) {
      
      printf("| ");
      
      phil = &phils[philCount];
      
      /*Print out the forks*/
      for (forkCount = 0; forkCount < NUM_PHILOSOPHERS; forkCount++) {
         fork = &forks[forkCount];
         
         if (fork->owner == phil) {
            printf("%d", fork->id);
         }
         else {
            printf("-");
         }
      }
      
      if (phil->state == CHANGING) {
         printf("       ");
      }
      else if (phil->state == THINKING) {
         printf(" Think ");
      }      
      else if (phil->state == EATING) {
         printf(" Eat   ");
      }
      else {
         fprintf(stderr, "phil %d has forbidden state %d", phil->id,
          phil->state);
         exit(-1);
      }   
   }
   printf("|\n");
      
   pthread_mutex_unlock(&print_mutex_lock);   
}

void printHeader() {
   printf("|=============|=============|=============|=============|======="
    "======|\n");
   printf("|      A      |      B      |      C      |      D      |      E"
    "      |\n");
   printf("|=============|=============|=============|=============|======="
    "======|\n"); 
}

void printFooter() {
   printf("|=============|=============|=============|=============|======="
    "======|\n");
}