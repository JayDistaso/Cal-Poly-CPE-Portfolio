typedef struct Rule {
   char *name;
   int capacityDepends;
   int capacityActions;
   int numDepends = 0;
   int numActions = 0;
   Dependency * depends;
   char * actions;
   int index;
} Rule;

typedef struct Dependency {
   char *name;
   unsigned short size;
   char * actions;
   int isRule;
   int index;
} Dependency;