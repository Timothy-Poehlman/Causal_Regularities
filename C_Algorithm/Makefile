PROG = causal
OBJS = Main.o pairlist.o conditionlist.o helper.o queue.o
CC = gcc
CFLAGS = -g -Wall

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(CFLAGS) $(OBJS)

clean:
	rm -f $(OBJS) causal.out

Main.o: pairlist.h Main.c conditionlist.h helper.h queue.h

helper.o: conditionlist.h queue.h

conditionlist.o: pairlist.h