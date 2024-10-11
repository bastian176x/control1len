#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructuras definidas aquí porque el .h solo las declara como 'struct'
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
int combustible = 0;

#define INFINITO 999999
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
