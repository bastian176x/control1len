# Variables
CC = gcc
CFLAGS = -Wall -g
LEX = flex
YACC = bison -d

# Archivos fuente
LEX_FILE = galaxia.l
YACC_FILE = galaxia.y
SRC_PART1 = galaxias.c
OBJS_PART1 = galaxia.tab.o lex.yy.o galaxias.o

# Archivos de salida
EXEC_PART1 = galaxia

# Reglas

all: $(EXEC_PART1)

# Compilar Parte 1 (galaxia)
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

clean:
	rm -f $(OBJS_PART1) $(EXEC_PART1) galaxia.tab.c galaxia.tab.h lex.yy.c
