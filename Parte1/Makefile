# Variables
CC = gcc
CFLAGS = -Wall -g
LEX = flex
YACC = bison -d

# Source files
LEX_FILE = galaxia.l
YACC_FILE = galaxia.y
UTILS_SRC = ../utils/utils.c
OBJS_PART1 = galaxia.tab.o lex.yy.o galaxias.o utils.o

# Output executable
EXEC_PART1 = galaxia

# Rules

all: $(EXEC_PART1)

$(EXEC_PART1): $(OBJS_PART1)
	$(CC) $(CFLAGS) -o $(EXEC_PART1) $(OBJS_PART1)

galaxia.tab.c galaxia.tab.h: $(YACC_FILE)
	$(YACC) $(YACC_FILE)

lex.yy.c: $(LEX_FILE)
	$(LEX) $(LEX_FILE)

galaxia.tab.o: galaxia.tab.c galaxia.tab.h
	$(CC) $(CFLAGS) -c galaxia.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

galaxias.o: galaxias.c galaxias.h
	$(CC) $(CFLAGS) -c galaxias.c

utils.o: $(UTILS_SRC)
	$(CC) $(CFLAGS) -c $(UTILS_SRC)

clean:
	rm -f $(OBJS_PART1) $(EXEC_PART1) galaxia.tab.c galaxia.tab.h lex.yy.c
