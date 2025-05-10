CC = gcc
CFLAGS = -Wall -g -Iinclude 

SRCDIR = src
OBJDIR = obj
INCLUDEDIR = include

SRCS = $(wildcard $(SRCDIR)/*.c) 
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

TARGET = program

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/* $(TARGET)
