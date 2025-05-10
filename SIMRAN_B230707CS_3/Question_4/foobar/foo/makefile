CC = gcc
CFLAGS = -fPIC -I../include
OBJS = foo1.o foo2.o foo3.o
.PHONY: all
all: $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 
