# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99 -g

# Target executable name
TARGET = stage1exe

# Object files
OBJS = driver.o lexer.o parser.o lookuptable.o tree.o utils.o

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Build object files
driver.o: driver.c lexer.h parser.h
	$(CC) $(CFLAGS) -c driver.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parser.c parser.h lexer.h
	$(CC) $(CFLAGS) -c parser.c

lookuptable.o: lookuptable.c lookuptable.h
	$(CC) $(CFLAGS) -c lookuptable.c

tree.o: tree.c tree.h parser.h
	$(CC) $(CFLAGS) -c tree.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

# Clean target to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)
