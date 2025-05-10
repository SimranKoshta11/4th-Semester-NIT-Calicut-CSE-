CC = gcc
CFLAGS = -fPIC -I../include
OBJS = bar1.o bar2.o
.PHONY: all
all: $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
