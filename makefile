CC		= gcc
LIB 	= -I
IDIR 	= include/
CFLAGS	= -Wall -I$(IDIR) -g -std=c11

BINDIR	= bin
OBJDIR	= objdir
SOURCES	= $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET	= HTTPRequestTest

.PHONY: clean all

all: dir $(TARGET)

dir:
	@mkdir -p objdir
	@mkdir -p bin

# Compile 
# Isue where $? doesnt list all of the dependancys 
$(TARGET):$(OBJ)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^

$(OBJDIR)/%.o:src/%.c
	$(CC) $(CFLAGS) -c $? -o $@ 

clean:
	@rm -f $(TARGET) $(wildcard *.o)
	@rm -rf $(OBJDIR) 
