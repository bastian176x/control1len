galaxia: galaxia.l galaxia.y
	bison -d galaxia.y
	flex galaxia.l
	gcc lex.yy.c galaxia.tab.c -o galaxia

clean:
	rm -f lex.yy.c galaxia.tab.c galaxia.tab.h galaxia
