PROG = causal
OBJS = Main.o pairlist.o conditionlist.o helper.o queue.o thread.o clqueue.o solutionlist.o
CC = gcc
CFLAGS = -g -Wall

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -pthread -o $(PROG) $(CFLAGS) $(OBJS)

clean:
	rm -f $(OBJS) causal.out

Main.o: Main.c conditionlist.h helper.h thread.h

helper.o: clqueue.h conditionlist.h queue.h

conditionlist.o: pairlist.h

