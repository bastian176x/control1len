%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "galaxias.h"  

extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);
%}



%union {
    int numero;
    char* str;
}

%token <str> GALAXIA NAVE ARISTA COMBUSTIBLE PESO SUBGALAXIA
%token <numero> NUMERO
%token <str> IDENTIFICADOR
%token PUNTOYCOMA COMA IGUAL REABASTECER VIAJAR AUTONOMO GUIADO

%type <str> definicion_galaxia definicion_nave definicion_arista ubicacion

%%

programa:
    definiciones
;

definiciones:
    definicion_galaxia
    | definicion_nave
    | definicion_arista
    | definiciones definicion_galaxia
    | definiciones definicion_nave
    | definiciones definicion_arista
;

definicion_galaxia:
    GALAXIA IDENTIFICADOR PUNTOYCOMA
    {
        if(buscarGalaxia(galaxias, $2) == NULL){
            galaxias = agregarGalaxia(galaxias, $2);
        }
    }
;

definicion_nave:
    NAVE IDENTIFICADOR COMA COMBUSTIBLE IGUAL NUMERO COMA ubicacion COMA REABASTECER PUNTOYCOMA
    {
        combustible = $6;
        ubicacion_nave = strdup($8);
        printf("Nave '%s' creada con %d unidades de combustible en la galaxia '%s'\n", $2, combustible, ubicacion_nave);
    }
;

definicion_arista:
    ARISTA IDENTIFICADOR COMA IDENTIFICADOR IGUAL PESO IGUAL NUMERO PUNTOYCOMA
    {
        Galaxia* origen = buscarGalaxia(galaxias, $2);
        Galaxia* destino = buscarGalaxia(galaxias, $4);
        if(origen && destino){
            agregarArista(origen, $4, $8);
        } else {
            printf("Error: Las galaxias %s o %s no existen.\n", $2, $4);
        }
    }
;

ubicacion:
    IDENTIFICADOR
    {
        $$ = $1;
    }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *archivo = fopen(argv[1], "r");
        if (!archivo) {
            perror("No se pudo abrir el archivo");
            return 1;
        }
        yyin = archivo;
    }
    yyparse();
    modificarPesoArista(galaxias);
    guardarDatos("salida.txt");
    return 0;
}
