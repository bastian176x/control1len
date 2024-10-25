%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "galaxias.h"
    #include "lex.yy.h"

    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;
    extern int yylineno;

    void yyerror(const char *s);

    int modo_interactivo = 0;
    int terminar = 0;  // Variable para controlar la terminación del bucle interactivo
%}

%union {
    int numero;
    char* str;
}

%token <str> GALAXIA ARISTA COMBUSTIBLE PESO SUBGALAXIA
%token <numero> NUMERO
%token <str> IDENTIFICADOR
%token PUNTOYCOMA COMA IGUAL REABASTECER VIAJAR AUTONOMO GUIADO MODIFICAR_ARISTA SALIR ERROR
%token <str> CREAR_NAVE

%type <str> definicion_galaxia definicion_arista ubicacion

%start programa_o_comando

%%

programa_o_comando:
    programa
    {
        /* Modo programa */
    }
    | comando
    {
        /* Modo comando */
    }
;

programa:
    definiciones_opt comandos_interactivos_opt
;

comando:
    comando_interactivo
;

definiciones_opt:
    /* vacío */
    | definiciones
;

definiciones:
    definicion
    | definiciones definicion
;

definicion:
    definicion_galaxia
    | definicion_arista
;

comandos_interactivos_opt:
    /* vacío */
    | comandos_interactivos
;

comandos_interactivos:
    comando_interactivo
    | comandos_interactivos comando_interactivo
;

definicion_galaxia:
    GALAXIA IDENTIFICADOR PUNTOYCOMA
    {
        if(buscarGalaxia(galaxias, $2) == NULL){
            galaxias = agregarGalaxia(galaxias, $2);
        }
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

comando_interactivo:
    MODIFICAR_ARISTA IDENTIFICADOR COMA IDENTIFICADOR IGUAL PESO IGUAL NUMERO PUNTOYCOMA
    {
        Galaxia* origen = buscarGalaxia(galaxias, $2);
        Galaxia* destino = buscarGalaxia(galaxias, $4);
        if (origen && destino) {
            modificarPesoArista(origen, $4, $8);
        } else {
            printf("Error: Las galaxias %s o %s no existen.\n", $2, $4);
        }
    }
    | CREAR_NAVE IDENTIFICADOR NUMERO IDENTIFICADOR PUNTOYCOMA
    {
        Galaxia* ubicacion = buscarGalaxia(galaxias, $4);
        if (ubicacion == NULL) {
            printf("Error: La galaxia '%s' no existe.\n", $4);
        } else {
            combustible = $3;
            nombre_nave = strdup($2);  // Guardar el nombre de la nave
            ubicacion_nave = strdup($4);
            printf("Nave '%s' creada con %d unidades de combustible en la galaxia '%s'\n", $2, $3, $4);
        }
    }
    | SALIR PUNTOYCOMA
    {
        printf("Comando: salir\n");
        terminar = 1;  // Indicar que se debe terminar el bucle interactivo
    }
    | error PUNTOYCOMA
        {
            printf("Error: Sintaxis incorrecta en el comando.\n");
            yyclearin;  // Limpiar el token actual
            yyerrok;    // Resetear el estado de error
        }
    | error
        {
            printf("Error: Comando no reconocido o sintaxis incorrecta.\n");
            yyclearin;  // Limpiar el token actual
            yyerrok;    // Resetear el estado de error
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
    fprintf(stderr, "Error de sintaxis en línea %d: %s\n", yylineno, s);
}

int parse_programa() {
    modo_interactivo = 0;
    return yyparse();
}

int parse_comando() {
    modo_interactivo = 1;
    return yyparse();
}

void procesarEntradaInteractiva() {
    char entrada[1024];
    while (!terminar) {
        printf("> ");
        fflush(stdout); // Asegurarse de que el prompt se muestra correctamente

        // Leer entrada interactiva del usuario
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            // Si se encuentra fin de archivo (Ctrl+D o fin de entrada)
            break;
        }

        // Remover el carácter de nueva línea al final si está presente
        size_t len = strlen(entrada);
        if (len > 0 && entrada[len - 1] == '\n') {
            entrada[len - 1] = '\0';
        }

        // Crear un nuevo buffer para la entrada leída
        YY_BUFFER_STATE bufferState = yy_scan_string(entrada);

        // Procesar la entrada
        int parse_result = parse_comando();
        if (parse_result != 0) {
            printf("Error al procesar el comando.\n");
            yyclearin; // Limpiar el token actual tras un error
        }

        // Limpiar el buffer para la siguiente entrada
        yy_delete_buffer(bufferState);

    }
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

    // Procesar el archivo de entrada
    if (parse_programa() != 0) {
        fprintf(stderr, "Error al procesar el archivo de entrada.\n");
        return 1;
    }

    // Modo interactivo
    printf("Sistema de navegacion.\n");
    printf("Escriba 'salir;' para terminar el programa.\n");

    // Procesar entrada interactiva
    procesarEntradaInteractiva();

    // Guardar los datos en el archivo de salida
    guardarDatos("salida.txt");

    return 0;
}
