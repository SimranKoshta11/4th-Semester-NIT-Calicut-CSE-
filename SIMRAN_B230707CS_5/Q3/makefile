CC = gcc
CFLAGS = -Wall -g

all: parent child dummy

parent: parent.c
	$(CC) $(CFLAGS) -o parent parent.c

child: child.c
	$(CC) $(CFLAGS) -o child child.c

dummy: dummy.c
	$(CC) $(CFLAGS) -o dummy dummy.c

clean:
	rm -f parent child dummy childpid.txt

