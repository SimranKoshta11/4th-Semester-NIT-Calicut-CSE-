CC = gcc
CFLAGS = -Wall -g -Iinclude -Wextra -Werror -std=c11 -MMD -MP

SRCDIR = src
OBJDIR = obj
DEPDIR = deps
INCLUDEDIR = include

SRCS = $(wildcard $(SRCDIR)/*.c) 
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
DEPS = $(patsubst $(SRCDIR)/%.c, $(DEPDIR)/%.d, $(SRCS))

TARGET = program

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) $(DEPDIR)  
	$(CC) $(CFLAGS) -c $< -o $@ 
	@mv $(OBJDIR)/$*.d $(DEPDIR)/

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)/* $(DEPDIR)/* $(TARGET)