make: liblwp.so

CC 	= gcc

CFLAGS  = -Wall -g -rdynamic -I .

LD 	= gcc

LDFLAGS  = -Wall -g -rdynamic -L.

PUBFILES =  README  hungrymain.c  libPLN.a  libsnakes.a  lwp.h\
	    numbersmain.c  snakemain.c  snakes.h

TARGET =  pn-cs453@hornet:Given/asgn2

PROGS	= snakes nums hungry

SNAKEOBJS  = snakemain.o 

HUNGRYOBJS = hungrymain.o 

NUMOBJS    = numbersmain.o

OBJS	= $(SNAKEOBJS) $(HUNGRYOBJS) $(NUMOBJS) 

SRCS	= snakemain.c numbersmain.c

HDRS	= 

EXTRACLEAN = core $(PROGS)

all: 	$(PROGS)

allclean: clean
	@rm -f $(EXTRACLEAN)

liblwp.so: allclean
	gcc -fPIC -c lwp.c
	gcc -o magic64.o -c magic64.S
	gcc -shared -o liblwp.so -fPIC lwp.o magic64.o

clean:	
	rm -f $(OBJS) *~ TAGS

snakes: snakemain.o libPLN.a libsnakes.a
	$(LD) $(LDFLAGS) -o snakes snakemain.o -L. -lncurses -lsnakes -lPLN

hungry: hungrymain.o libPLN.a libsnakes.a
	$(LD) $(LDFLAGS) -o hungry hungrymain.o -L. -lncurses -lsnakes -lPLN

nums: numbersmain.o libPLN.a 
	$(LD) $(LDFLAGS) -o nums numbersmain.o -L. -lPLN

hungrymain.o: lwp.h snakes.h

snakemain.o: lwp.h snakes.h

numbermain.o: lwp.h

libPLN.a: ./lwp.c
	gcc -c -g ./lwp.c ./lwp.h
	gcc -o -g magic64.o -c magic64.S
	ar r libPLN.a lwp.o magic64.o
	rm lwp.o

pub:
	scp $(PUBFILES) $(TARGET)

