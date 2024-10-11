%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "galaxia.tab.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
void yyerror(const char *s);
int combustible = 0; // Combustible de la nave
char* ubicacion_nave = NULL; // Ubicación actual de la nave

// Definiciones de las estructuras en Parte 1 (no se pueden eliminar)
typedef struct Arista {
    char* destino;
    int peso;
    struct Arista* siguiente;
} Arista;

typedef struct Galaxia {
    char* nombre;
    Arista* adyacencias;
    struct Galaxia* siguiente;
} Galaxia;

Galaxia* galaxias = NULL;

Galaxia* buscarGalaxia(Galaxia* lista, char* nombre);
Galaxia* agregarGalaxia(Galaxia* lista, char* nombre);
void agregarArista(Galaxia* galaxia, char* destino, int peso);

// Implementación de las funciones del grafo
Galaxia* buscarGalaxia(Galaxia* lista, char* nombre) {
    Galaxia* actual = lista;
    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre) == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

Galaxia* agregarGalaxia(Galaxia* lista, char* nombre) {
    Galaxia* nueva = (Galaxia*)malloc(sizeof(Galaxia));
    nueva->nombre = strdup(nombre);
    nueva->adyacencias = NULL;
    nueva->siguiente = lista;
    return nueva;
}

void agregarArista(Galaxia* galaxia, char* destino, int peso) {
    Arista* nueva = (Arista*)malloc(sizeof(Arista));
    nueva->destino = strdup(destino);
    nueva->peso = peso;
    nueva->siguiente = galaxia->adyacencias;
    galaxia->adyacencias = nueva;

    Galaxia* destinoGalaxia = buscarGalaxia(galaxias, destino);
    if (destinoGalaxia != NULL) {
        Arista* aristaInversa = (Arista*)malloc(sizeof(Arista));
        aristaInversa->destino = strdup(galaxia->nombre);
        aristaInversa->peso = peso;
        aristaInversa->siguiente = destinoGalaxia->adyacencias;
        destinoGalaxia->adyacencias = aristaInversa;
    } else {
        printf("Error: La galaxia destino %s no existe.\n", destino);
    }

    printf("Arista agregada de %s a %s con peso %d\n", galaxia->nombre, destino, peso);
    printf("Arista agregada de %s a %s con peso %d\n", destino, galaxia->nombre, peso);
}

// Función para guardar datos en un archivo
void guardarDatos(const char* nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        perror("Error al abrir archivo para guardar datos");
        return;
    }

    Galaxia* actualGalaxia = galaxias;
    while (actualGalaxia != NULL) {
        fprintf(archivo, "Galaxia: %s\n", actualGalaxia->nombre);
        Arista* actualArista = actualGalaxia->adyacencias;
        while (actualArista != NULL) {
            fprintf(archivo, "  Arista hacia: %s, Peso: %d\n", actualArista->destino, actualArista->peso);
            actualArista = actualArista->siguiente;
        }
        actualGalaxia = actualGalaxia->siguiente;
    }

    fprintf(archivo, "Nave: Combustible: %d, Ubicación: %s\n", combustible, ubicacion_nave);
    fclose(archivo);
}
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
        if(buscarGalaxia(galaxias,$2) == NULL){
            galaxias = agregarGalaxia(galaxias,$2);
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
        Galaxia* origen = buscarGalaxia(galaxias,$2);
        Galaxia* destino = buscarGalaxia(galaxias, $4);
        if(origen && destino){
            agregarArista(origen, $4, $8);
        }else{
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
    guardarDatos("salida.txt");
    return 0;
}
