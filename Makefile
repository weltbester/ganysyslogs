CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Wwrite-strings -O3
DBGFLAGS = -std=c11 -pedantic -Wall -Wwrite-strings -ggdb3 -DDEBUG
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
DBGOBJS=$(patsubst %.c,%.dbg.o,$(SRCS))
.PHONY: clean depend all
all: ganysyslogs ganysyslogs-debug
ganysyslogs: $(OBJS)
	   gcc -o $@ -O3 $(OBJS)
ganysyslogs-debug: $(DBGOBJS)
		 gcc -o $@ -ggdb3 $(DBGOBJS)
%.dbg.o: %.c
	 gcc $(DBGFLAGS) -c -o $@ $<
clean:
	rm -f ganysyslogs ganysyslogs-debug *.o *.c~ *.h~
depend:
	makedepend $(SRCS)
	makedepend -a -o .dbg.o  $(SRCS)
# DO NOT DELETE
