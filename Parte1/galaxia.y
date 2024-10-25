%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parte2.tab.h"
#include "../Parte1/galaxias.h"
void cargarDatos(const char* nombreArchivo);
#define MAX_COMBUSTIBLE 30  // Capacidad máxima de combustible


// Declarar funciones
void viajar(const char* destino);
void mostrar_combustible();
void reabastecer_combustible();
void ruta_optima(const char* origen, const char* destino);
void ruta_corta(const char* origen, const char* destino);
void mostrar_ayuda();
void secuenciaDeViaje(Galaxia* galaxiaActual, const char* secuencia);
void mostrarGalaxiasVecinas(Galaxia* galaxia, int radio);
void mostrar_galaxia_actual();

// Variables globales
extern int combustible;
extern char* ubicacion_nave;
extern Galaxia* galaxias;

int yylex(void);
void yyerror(const char* s) {
    if (yylval.strval) {
        fprintf(stderr, "Error: %s en el token con valor '%s'\n", s, yylval.strval);
    } else {
        fprintf(stderr, "Error: %s en el token con valor '%d'\n", s, yylval.intval);
    }
}

%}

%union {
    char* strval;
    int intval;
}

%token VIAJAR COMBUSTIBLE REABASTECER RUTA_OPTIMA RUTA_CORTA MOSTRAR_VECINAS VIAJAR_SECUENCIA HELP GALAXIA_ACTUAL

%token <strval> IDENTIFICADOR
%token <intval> NUMERO

%%

inicio:
    comandos
;

comandos:
    comandos comando '\n'
    |
;

comando:
    VIAJAR IDENTIFICADOR {
        viajar($2);
        free($2);
    }
    |
    VIAJAR_SECUENCIA IDENTIFICADOR {
        secuenciaDeViaje(buscarGalaxia(galaxias, ubicacion_nave), $2);
        free($2);
    }
    |
    COMBUSTIBLE {
        mostrar_combustible();
    }
    |
    REABASTECER {
        reabastecer_combustible();
    }
    |
    RUTA_OPTIMA IDENTIFICADOR IDENTIFICADOR {
        ruta_optima($2, $3);
        free($2);
        free($3);
    }
    |
    RUTA_CORTA IDENTIFICADOR IDENTIFICADOR {
        ruta_corta($2, $3);
        free($2);
        free($3);
    }
    |
    MOSTRAR_VECINAS NUMERO {
        printf("El radio ingresado es: %d\n", $2);
        mostrarGalaxiasVecinas(buscarGalaxia(galaxias, ubicacion_nave), $2);
    }

    |
    GALAXIA_ACTUAL {
        mostrar_galaxia_actual();
    }
    |
    HELP {
        mostrar_ayuda();
    }
;

%%

int main() {
    // Cargar los datos desde el archivo generado
    cargarDatos("../Parte1/salida.txt");
    
    // Entrar en modo comando
    printf("Bienvenido a la línea de comandos de la nave espacial.\n");
    yyparse();
    
    return 0;
}