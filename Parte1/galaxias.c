// galaxias.c

#include "galaxias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definir las variables globales aquí
Galaxia* galaxias = NULL;
char* ubicacion_nave = NULL;
char* nombre_nave = NULL;  // Variable para almacenar el nombre de la nave
int combustible = 0;

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
    printf("Galaxia '%s' agregada correctamente.\n", nombre);
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

void modificarPesoArista(Galaxia* origen, char* destino, int nuevoPeso) {
    Arista* actual = origen->adyacencias;
    while (actual != NULL) {
        if (strcmp(actual->destino, destino) == 0) {
            printf("Modificando peso de la arista de %s a %s, peso anterior: %d, nuevo peso: %d\n", 
                   origen->nombre, destino, actual->peso, nuevoPeso);
            actual->peso = nuevoPeso;
            return;
        }
        actual = actual->siguiente;
    }
    printf("La arista entre %s y %s no existe.\n", origen->nombre, destino);
}

void guardarDatos(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        perror("Error al abrir archivo para guardar datos");
        return;
    }

    // **Ordenar las galaxias alfabéticamente**
    // Contar el número de galaxias
    int numGalaxias = 0;
    Galaxia* actualGalaxia = galaxias;
    while (actualGalaxia != NULL) {
        numGalaxias++;
        actualGalaxia = actualGalaxia->siguiente;
    }

    // Crear un arreglo para las galaxias
    Galaxia** arregloGalaxias = (Galaxia**)malloc(numGalaxias * sizeof(Galaxia*));
    actualGalaxia = galaxias;
    for (int i = 0; i < numGalaxias; i++) {
        arregloGalaxias[i] = actualGalaxia;
        actualGalaxia = actualGalaxia->siguiente;
    }

    // Ordenar el arreglo de galaxias
    for (int i = 0; i < numGalaxias - 1; i++) {
        for (int j = i + 1; j < numGalaxias; j++) {
            if (strcmp(arregloGalaxias[i]->nombre, arregloGalaxias[j]->nombre) > 0) {
                Galaxia* temp = arregloGalaxias[i];
                arregloGalaxias[i] = arregloGalaxias[j];
                arregloGalaxias[j] = temp;
            }
        }
    }

    // Guardar las galaxias en el archivo
    for (int i = 0; i < numGalaxias; i++) {
        fprintf(archivo, "galaxia %s;\n", arregloGalaxias[i]->nombre);
    }

    // **Guardar las aristas**
    // Para evitar duplicados, almacenamos las aristas en un arreglo y las ordenamos
    typedef struct {
        char* origen;
        char* destino;
        int peso;
    } AristaInfo;

    int maxAristas = 1000; // Asumiendo que no habrá más de 1000 aristas
    AristaInfo* aristas = (AristaInfo*)malloc(maxAristas * sizeof(AristaInfo));
    int numAristas = 0;

    for (int i = 0; i < numGalaxias; i++) {
        actualGalaxia = arregloGalaxias[i];
        Arista* actualArista = actualGalaxia->adyacencias;
        while (actualArista != NULL) {
            // Para evitar duplicados, solo agregamos la arista si origen < destino
            if (strcmp(actualGalaxia->nombre, actualArista->destino) < 0) {
                aristas[numAristas].origen = actualGalaxia->nombre;
                aristas[numAristas].destino = actualArista->destino;
                aristas[numAristas].peso = actualArista->peso;
                numAristas++;
            }
            actualArista = actualArista->siguiente;
        }
    }

    // Ordenar las aristas alfabéticamente por origen y luego por destino
    for (int i = 0; i < numAristas - 1; i++) {
        for (int j = i + 1; j < numAristas; j++) {
            int cmpOrigen = strcmp(aristas[i].origen, aristas[j].origen);
            if (cmpOrigen > 0 || (cmpOrigen == 0 && strcmp(aristas[i].destino, aristas[j].destino) > 0)) {
                AristaInfo temp = aristas[i];
                aristas[i] = aristas[j];
                aristas[j] = temp;
            }
        }
    }

    // Guardar las aristas en el archivo
    for (int i = 0; i < numAristas; i++) {
        fprintf(archivo, "arista %s, %s = peso = %d;\n", aristas[i].origen, aristas[i].destino, aristas[i].peso);
    }

    // Liberar memoria de las aristas
    free(aristas);
    free(arregloGalaxias);

    // Guardar los datos de la nave en el formato correcto
    if (nombre_nave != NULL && ubicacion_nave != NULL) {
        fprintf(archivo, "\nnave %s, combustible = %d, %s, reabastecer;\n", nombre_nave, combustible, ubicacion_nave);
    }

    fclose(archivo);
}
