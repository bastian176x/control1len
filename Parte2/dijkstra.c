#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // Necesario para isspace
#include "../Parte1/galaxias.h"
#include "../utils/utils.h"
  // Incluir el archivo de cabecera de la Parte 1

#define INFINITO 999999


// Funciones relacionadas con el algoritmo de Dijkstra (puedes reutilizar las otras desde galaxia.h)
Galaxia* encontrarMenorDistancia(Galaxia* lista, int* distancias, int* visitados);
void dijkstra(Galaxia* lista, char* inicio, char* destino);
void cargarDatos(const char* nombreArchivo);




// Función para cargar los datos desde el archivo generado (reutilizando agregarGalaxia, agregarArista, etc.)
void cargarDatos(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo de salida");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        if (strncmp(linea, "galaxia ", 8) == 0) {
            char nombreGalaxia[100];
            sscanf(linea, "galaxia %[^;];", nombreGalaxia);
            trimWhitespace(nombreGalaxia);  // Recortar espacios en blanco
            if (!buscarGalaxia(galaxias, nombreGalaxia)) {
                galaxias = agregarGalaxia(galaxias, nombreGalaxia);
            }
        } else if (strncmp(linea, "arista ", 7) == 0) {
            char origen[100], destino[100];
            int peso;
            sscanf(linea, "arista %[^,], %[^=]= peso = %d;", origen, destino, &peso);

            trimWhitespace(origen);   // Recortar espacios en blanco
            trimWhitespace(destino);  // Recortar espacios en blanco

            Galaxia* origenGalaxia = buscarGalaxia(galaxias, origen);
            Galaxia* destinoGalaxia = buscarGalaxia(galaxias, destino);

            if (origenGalaxia == NULL || destinoGalaxia == NULL) {
                printf("Error: Galaxia de origen '%s' o destino '%s' no existe\n", origen, destino);
                continue;
            }

            agregarArista(origenGalaxia, destino, peso);
        } else if (strncmp(linea, "nave ", 5) == 0) {
            char nombreNave[100], ubicacion[100];
            sscanf(linea, "nave %[^,], combustible = %d, %[^,], reabastecer;", nombreNave, &combustible, ubicacion);
            trimWhitespace(nombreNave);    // Recortar espacios en blanco
            trimWhitespace(ubicacion);     // Recortar espacios en blanco
            ubicacion_nave = strdup(ubicacion);
        }
    }

    fclose(archivo);
}




// Implementación del algoritmo de Dijkstra
Galaxia* encontrarMenorDistancia(Galaxia* lista, int* distancias, int* visitados) {
    Galaxia* menorNodo = NULL;
    int menorDistancia = INFINITO;
    Galaxia* actual = lista;
    int index = 0;
    while (actual != NULL) {
        if (!visitados[index] && distancias[index] < menorDistancia) {
            menorDistancia = distancias[index];
            menorNodo = actual;
        }
        actual = actual->siguiente;
        index++;
    }
    return menorNodo;
}

void dijkstra(Galaxia* lista, char* inicio, char* destino) {
    int distancias[100];          // Para almacenar la distancia más corta a cada galaxia
    int visitados[100] = {0};     // Marcar galaxias ya visitadas
    Galaxia* predecesores[100] = {NULL}; // Para almacenar el predecesor de cada galaxia
    Galaxia* nodos[100];          // Lista de galaxias por índice para facilitar el acceso
    int index = 0;

    // Inicializar distancias a infinito y predecesores a NULL
    Galaxia* actual = lista;
    while (actual != NULL) {
        distancias[index] = INFINITO;
        nodos[index] = actual;
        actual = actual->siguiente;
        index++;
    }

    int num_nodos = index; // Número total de galaxias

    // La distancia al nodo de inicio es 0
    int idxInicio = -1;
    for (int i = 0; i < num_nodos; i++) {
        if (strcmp(nodos[i]->nombre, inicio) == 0) {
            idxInicio = i;
            break;
        }
    }
    if (idxInicio == -1) {
        printf("Error: La galaxia de inicio no existe\n");
        return;
    }

    int idxDestino = -1;
    for (int i = 0; i < num_nodos; i++) {
        if (strcmp(nodos[i]->nombre, destino) == 0) {
            idxDestino = i;
            break;
        }
    }
    if (idxDestino == -1) {
        printf("Error: La galaxia de destino no existe\n");
        return;
    }

    distancias[idxInicio] = 0;

    // Mientras haya nodos no visitados con distancia finita
    while ((actual = encontrarMenorDistancia(lista, distancias, visitados)) != NULL) {
        int idxActual = -1;
        for (int i = 0; i < num_nodos; i++) {
            if (nodos[i] == actual) {
                idxActual = i;
                break;
            }
        }

        visitados[idxActual] = 1;

        Arista* arista = actual->adyacencias;
        while (arista != NULL) {
            int idxDestinoArista = -1;
            for (int i = 0; i < num_nodos; i++) {
                if (strcmp(nodos[i]->nombre, arista->destino) == 0) {
                    idxDestinoArista = i;
                    break;
                }
            }

            if (idxDestinoArista != -1 && !visitados[idxDestinoArista] && distancias[idxActual] + arista->peso < distancias[idxDestinoArista]) {
                distancias[idxDestinoArista] = distancias[idxActual] + arista->peso;
                predecesores[idxDestinoArista] = actual;
            }

            arista = arista->siguiente;
        }
    }

    if (distancias[idxDestino] == INFINITO) {
        printf("No existe un camino de %s a %s.\n", inicio, destino);
    } else {
        printf("La distancia mas corta de %s a %s es: %d\n", inicio, destino, distancias[idxDestino]);

        Galaxia* camino = nodos[idxDestino];
        printf("El camino mas corto es: %s", destino);
        
        while (camino != nodos[idxInicio]) {
            for (int i = 0; i < num_nodos; i++) {
                if (nodos[i] == camino) {
                    Galaxia* predecesor = predecesores[i];
                    if (predecesor == NULL) {
                        printf(" <- (Camino no válido)\n");
                        return;
                    }

                    // Encontrar la arista entre el predecesor y el nodo actual (camino)
                    Arista* arista = predecesor->adyacencias;
                    while (arista != NULL) {
                        if (strcmp(arista->destino, camino->nombre) == 0) {
                            printf(" -> %s", predecesor->nombre);
                            // Restar el peso al combustible de la nave
                            combustible -= arista->peso;
                            printf(" (combustible restante: %d)", combustible);

                            // Si el combustible llega a cero o menos, la nave no puede continuar
                            if (combustible <= 0) {
                                printf("\nLa nave se ha quedado sin combustible y no puede continuar.\n");
                                return;
                            }

                            break;
                        }
                        arista = arista->siguiente;
                    }

                    camino = predecesor;
                    break;
                }
            }
        }
        printf("\n");
    }
}

int main() {
    // Cargar los datos desde el archivo generado
    cargarDatos("../Parte1/salida.txt");

    // Imprimir el estado de la nave
    printf("La nave está en %s con %d unidades de combustible.\n", ubicacion_nave, combustible);

    // Ejecutar el algoritmo de Dijkstra
    char destino[100];
    printf("Ingrese el destino para la nave: ");
    scanf("%s", destino);

    dijkstra(galaxias, ubicacion_nave, destino);

    return 0;
 }