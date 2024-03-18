CC = gcc
CFLAGS = -g -O0 -lreadline
LDFLAGS = -g
SRCS = step0_repl.c

OBJS = $(SRCS:%.c=%.o)
BINS = $(OBJS:%.o=%)

all: myc

myc: $(OBJS)
	$(CC) $(CFLAGS) -o run $(OBJS)

.c.o: $(SRCS)
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -f *.o run
