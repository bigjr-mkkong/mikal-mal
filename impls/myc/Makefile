CC = gcc
CFLAGS = -g -O0 -lreadline
LDFLAGS = -g
SRCS = step1_read_print.c reader.c

OBJS = $(SRCS:%.c=%.o)
BINS = $(OBJS:%.o=%)

all: myc

myc: $(OBJS)
	$(CC) $(CFLAGS) -o run $(OBJS)

.c.o: $(SRCS)
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -f *.o run
