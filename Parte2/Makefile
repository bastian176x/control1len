all: dijkstra

dijkstra: lex.yy.c parte2.tab.c dijkstra.c ../Parte1/galaxias.c ../utils/utils.c
	 gcc -o dijkstra lex.yy.c parte2.tab.c dijkstra.c ../Parte1/galaxias.c ../utils/utils.c

parte2.tab.c: parte2.y
	 bison -d parte2.y

lex.yy.c: parte2.l
	 flex parte2.l
