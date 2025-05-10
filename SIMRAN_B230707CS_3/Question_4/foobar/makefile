CC = gcc
CFLAGS = -fPIC -Iinclude
SUBDIRS = foo bar
OBJS = foo/foo1.o foo/foo2.o foo/foo3.o bar/bar1.o bar/bar2.o
TARGET = libfoobar.so
.PHONY: all $(SUBDIRS)
all: $(SUBDIRS)
	$(CC) -shared -o $(TARGET) $(OBJS)
$(SUBDIRS):
	$(MAKE) -C $@
