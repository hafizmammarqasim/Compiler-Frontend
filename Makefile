# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I./src

# Source files
SRC = src/main.c \
      src/lexer/lexer.c \
      src/parser/parser.c \
      src/semantic/semantic.c \
      src/symbol_table/symbol_table.c

# Target executable
compiler:
	$(CC) $(CFLAGS) $(SRC) -o compiler

# Cleanup compiled files
clean:
	rm -f compiler
