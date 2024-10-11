#include "galaxias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  
#include "../utils/utils.h"



// Definición de variables globales
Galaxia* galaxias = NULL;
int combustible = 0;
char* ubicacion_nave = NULL;

// Implementación de las funciones

Galaxia* agregarGalaxia(Galaxia* lista, char* nombre) {
    Galaxia* nueva = (Galaxia*)malloc(sizeof(Galaxia));
    if (nueva == NULL) {
        printf("Error: No se pudo asignar memoria para la galaxia %s\n", nombre);
        return lista;
    }
    nueva->nombre = strdup(nombre);
    nueva->adyacencias = NULL;
    nueva->siguiente = lista;
    printf("Galaxia '%s' agregada correctamente.\n", nombre);  // Depuración para confirmar adición
    return nueva;
}




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


void agregarArista(Galaxia* galaxia, char* destino, int peso) {
    if (galaxia == NULL || destino == NULL) {
        printf("Error: Galaxia o destino nulo en agregarArista\n");
        return;
    }
    trimWhitespace(destino);
    
    Galaxia* destinoGalaxia = buscarGalaxia(galaxias, destino);
    if (destinoGalaxia == NULL) {
        printf("Error: No se puede agregar arista porque la galaxia destino %s no fue encontrada.\n", destino);
        return;
    }
    
    printf("Agregando arista de %s a %s con peso %d\n", galaxia->nombre, destino, peso);

    Arista* nueva = (Arista*)malloc(sizeof(Arista));
    if (nueva == NULL) {
        printf("Error al asignar memoria para nueva arista\n");
        return;
    }

    nueva->destino = strdup(destino);
    nueva->peso = peso;
    nueva->siguiente = galaxia->adyacencias;
    galaxia->adyacencias = nueva;

    // Agregar la arista inversa en el destino
    Arista* aristaInversa = (Arista*)malloc(sizeof(Arista));
    if (aristaInversa == NULL) {
        printf("Error al asignar memoria para arista inversa\n");
        return;
    }
    aristaInversa->destino = strdup(galaxia->nombre);
    aristaInversa->peso = peso;
    aristaInversa->siguiente = destinoGalaxia->adyacencias;
    destinoGalaxia->adyacencias = aristaInversa;
}




void guardarDatos(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        perror("Error al abrir archivo para guardar datos");
        return;
    }

    // Guardar las galaxias en el formato "galaxia X;"
    Galaxia* actualGalaxia = galaxias;
    while (actualGalaxia != NULL) {
        fprintf(archivo, "galaxia %s;\n", actualGalaxia->nombre);
        actualGalaxia = actualGalaxia->siguiente;
    }

    // Guardar las aristas en el formato "arista X, Y = peso = Z;"
    actualGalaxia = galaxias;
    while (actualGalaxia != NULL) {
        Arista* actualArista = actualGalaxia->adyacencias;
        while (actualArista != NULL) {
            // Para evitar duplicar las aristas (ambos sentidos), imprime solo si la galaxia actual es lexicográficamente menor que la galaxia destino
            if (strcmp(actualGalaxia->nombre, actualArista->destino) < 0) {
                fprintf(archivo, "arista %s, %s = peso = %d;\n", actualGalaxia->nombre, actualArista->destino, actualArista->peso);
            }
            actualArista = actualArista->siguiente;
        }
        actualGalaxia = actualGalaxia->siguiente;
    }

    // Guardar los datos de la nave en el formato correcto
    fprintf(archivo, "\nnave Enterprise, combustible = %d, %s, reabastecer;\n", combustible, ubicacion_nave);

    fclose(archivo);
}





void modificarPesoArista(Galaxia* galaxias) {
    char confirmacion;
    char origen[100];
    char destino[100];
    int nuevoPeso;

    printf("Desea modificar el peso de alguna arista? (s/n): ");
    scanf(" %c", &confirmacion);

    if (confirmacion == 's' || confirmacion == 'S') {
        printf("Ingrese el nombre de la galaxia de origen (solo se permiten galaxias adyacentes): ");
        scanf("%s", origen);
        Galaxia* galaxiaOrigen = buscarGalaxia(galaxias, origen);
        if (galaxiaOrigen == NULL) {
            printf("La galaxia de origen no existe.\n");
            return;
        }

        printf("Ingrese el nombre de la galaxia de destino (solo se permiten galaxias adyacentes): ");
        scanf("%s", destino);

        Arista* actual = galaxiaOrigen->adyacencias;
        while (actual != NULL) {
            if (strcmp(actual->destino, destino) == 0) {
                printf("Se ha encontrado la arista de %s a %s con peso actual %d.\n", origen, destino, actual->peso);
                printf("Ingrese el nuevo peso para la arista de %s a %s: ", origen, destino);
                scanf("%d", &nuevoPeso);
                actual->peso = nuevoPeso;
                printf("Peso de la arista modificado exitosamente.\n");
                return;
            }
            actual = actual->siguiente;
        }
        printf("La arista entre %s y %s no existe.\n", origen, destino);
    } else {
        printf("Modificacion cancelada.\n");
    }
}