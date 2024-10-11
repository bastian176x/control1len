%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Parte1/galaxias.h"  // Incluir el archivo de cabecera de la Parte 1

void cargarGalaxia(char* nombre);
void cargarArista(char* origen, char* destino, int peso);
void cargarNave(int combustible, char* ubicacion);

int yylex();

void cargarGalaxia(char* nombre) {
    galaxias = agregarGalaxia(galaxias, nombre);
}

void cargarArista(char* origen, char* destino, int peso) {
    agregarArista(buscarGalaxia(galaxias, origen), destino, peso);
}

void cargarNave(int combust, char* ubicacion) {
    combustible = combust;  // Asigna directamente el valor a la variable global
    if (ubicacion_nave != NULL) {
        free(ubicacion_nave);  // Libera la memoria previa si la variable ya tenía un valor
    }
    ubicacion_nave = strdup(ubicacion);  // Duplica el valor de la ubicación para asignarlo correctamente
}

void yyerror(const char* s) {
    fprintf(stderr, "Error de sintaxis: %s\n", s);
}
%}

%union {
    int intval;
    char* strval;
}

%token GALAXIA ARISTA NAVE REABASTECER
%token <strval> IDENTIFICADOR
%token <intval> NUMERO

%%

inicio:
    lista_galaxias nave
;

lista_galaxias:
    galaxia lista_galaxias
    | /* vacío */
;

galaxia:
    GALAXIA IDENTIFICADOR { cargarGalaxia($2); }
    lista_aristas
;

lista_aristas:
    arista lista_aristas
    | /* vacío */
;

arista:
    ARISTA IDENTIFICADOR IDENTIFICADOR NUMERO { cargarArista($2, $3, $4); }
;

nave:
    NAVE IDENTIFICADOR NUMERO IDENTIFICADOR REABASTECER { cargarNave($3, $2); }
;
%%
