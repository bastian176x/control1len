%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Parte1/galaxias.h" 

void cargarGalaxia(char* nombre);
void cargarArista(char* origen, char* destino, int peso);
void cargarNave(char* nombre, int combust, char* ubicacion);

int yylex();

void cargarGalaxia(char* nombre) {
    galaxias = agregarGalaxia(galaxias, nombre);
}

void cargarArista(char* origen, char* destino, int peso) {
    agregarArista(buscarGalaxia(galaxias, origen), destino, peso);
}

void cargarNave(char* nombre, int combust, char* ubicacion) {
    combustible = combust;
    if (ubicacion_nave != NULL) {
        free(ubicacion_nave);
    }
    ubicacion_nave = strdup(ubicacion);
    // El modo de viaje se establecerá más adelante
    printf("Nave '%s' creada con %d unidades de combustible en la galaxia '%s'\n",
           nombre, combustible, ubicacion_nave);
}


void yyerror(const char* s) {
    fprintf(stderr, "Error de sintaxis: %s\n", s);
}
%}

%union {
    int intval;
    char* strval;
}

%token GALAXIA ARISTA NAVE REABASTECER AUTONOMO GUIADO
%token <strval> IDENTIFICADOR
%token <intval> NUMERO

%type <intval> modo_viaje

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
    NAVE IDENTIFICADOR NUMERO IDENTIFICADOR REABASTECER { cargarNave($2, $3, $4); }
;


modo_viaje:
    AUTONOMO { $$ = 0; }
    | GUIADO { $$ = 1; }
;

%%
