// dijkstra.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../Parte1/galaxias.h"
#include "../utils/utils.h"

#define INFINITO 999999
#define MAX_COMBUSTIBLE 500  // Capacidad máxima de combustible

// Declaración de variables globales
Galaxia* galaxias = NULL;
char* ubicacion_nave = NULL;
int combustible = MAX_COMBUSTIBLE;
int modo_viaje = 0; // 0 para autónomo, 1 para guiado

// Declaración de funciones
void cargarDatos(const char* nombreArchivo);
void dijkstraConCombustible(Galaxia* lista, char* inicio, char* destino);
void shortestPathByGalaxies(Galaxia* lista, char* inicio, char* destino);
void viajeGuiado();
void mostrarGalaxiasVecinas(Galaxia* galaxia, int radio);
int esVecina(Galaxia* galaxia, char* nombreDestino);
int obtenerPesoArista(Galaxia* origen, char* destino);
void secuenciaDeViaje(Galaxia* galaxiaActual);
int esEstacionReabastecimiento(char* nombreGalaxia);

// Galaxias donde se puede reabastecer
const char* estacionesReabastecimiento[] = {"H", "G", "B"};
const int numEstacionesReabastecimiento = 3;

// Implementación de funciones

// Función para verificar si una galaxia es una estación de reabastecimiento
int esEstacionReabastecimiento(char* nombreGalaxia) {
    for (int i = 0; i < numEstacionesReabastecimiento; i++) {
        if (strcmp(nombreGalaxia, estacionesReabastecimiento[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Función para cargar los datos desde el archivo generado
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
            trimWhitespace(nombreGalaxia);
            if (!buscarGalaxia(galaxias, nombreGalaxia)) {
                galaxias = agregarGalaxia(galaxias, nombreGalaxia);
            }
        } else if (strncmp(linea, "arista ", 7) == 0) {
            char origen[100], destino[100];
            int peso;
            sscanf(linea, "arista %[^,], %[^=]= peso = %d;", origen, destino, &peso);

            trimWhitespace(origen);
            trimWhitespace(destino);

            Galaxia* origenGalaxia = buscarGalaxia(galaxias, origen);
            Galaxia* destinoGalaxia = buscarGalaxia(galaxias, destino);

            if (origenGalaxia == NULL || destinoGalaxia == NULL) {
                printf("Error: Galaxia de origen '%s' o destino '%s' no existe\n", origen, destino);
                continue;
            }

            agregarArista(origenGalaxia, destino, peso);
        } else if (strncmp(linea, "nave ", 5) == 0) {
            char nombreNave[100], ubicacion[100];
            int combust;
            sscanf(linea, "nave %[^,], combustible = %d, %[^,], reabastecer;", nombreNave, &combust, ubicacion);
            trimWhitespace(nombreNave);
            trimWhitespace(ubicacion);
            ubicacion_nave = strdup(ubicacion);
            combustible = combust;  // Asigna el combustible leído
            if (combustible > MAX_COMBUSTIBLE) {
                combustible = MAX_COMBUSTIBLE;  // No exceder el máximo
            }
        }
    }

    fclose(archivo);
}

// Implementación del algoritmo de Dijkstra con manejo de combustible y restricción de reabastecimiento
void dijkstraConCombustible(Galaxia* lista, char* inicio, char* destino) {
    int distancias[100];
    int visitados[100] = {0};
    Galaxia* predecesores[100] = {NULL};
    Galaxia* nodos[100];
    int index = 0;

    // Inicializar distancias a infinito y predecesores a NULL
    Galaxia* actual = lista;
    while (actual != NULL) {
        distancias[index] = INFINITO;
        nodos[index] = actual;
        actual = actual->siguiente;
        index++;
    }

    int num_nodos = index;

    // Encontrar los índices de inicio y destino
    int idxInicio = -1;
    int idxDestino = -1;
    for (int i = 0; i < num_nodos; i++) {
        if (strcmp(nodos[i]->nombre, inicio) == 0) {
            idxInicio = i;
        }
        if (strcmp(nodos[i]->nombre, destino) == 0) {
            idxDestino = i;
        }
    }

    if (idxInicio == -1) {
        printf("Error: La galaxia de inicio no existe\n");
        return;
    }
    if (idxDestino == -1) {
        printf("Error: La galaxia de destino no existe\n");
        return;
    }

    distancias[idxInicio] = 0;

    // Algoritmo de Dijkstra
    while (1) {
        // Encontrar el nodo no visitado con la menor distancia
        int menorDistancia = INFINITO;
        int idxActual = -1;
        for (int i = 0; i < num_nodos; i++) {
            if (!visitados[i] && distancias[i] < menorDistancia) {
                menorDistancia = distancias[i];
                idxActual = i;
            }
        }

        if (idxActual == -1) {
            // No hay más nodos alcanzables
            break;
        }

        visitados[idxActual] = 1;
        actual = nodos[idxActual];

        // Si hemos llegado al destino, terminamos
        if (idxActual == idxDestino) {
            break;
        }

        Arista* arista = actual->adyacencias;
        while (arista != NULL) {
            // Encontrar el índice de la galaxia adyacente
            int idxAdyacente = -1;
            for (int i = 0; i < num_nodos; i++) {
                if (strcmp(nodos[i]->nombre, arista->destino) == 0) {
                    idxAdyacente = i;
                    break;
                }
            }
            if (idxAdyacente != -1) {
                int nuevaDistancia = distancias[idxActual] + arista->peso;
                if (nuevaDistancia < distancias[idxAdyacente]) {
                    distancias[idxAdyacente] = nuevaDistancia;
                    predecesores[idxAdyacente] = actual;
                }
            }
            arista = arista->siguiente;
        }
    }

    if (distancias[idxDestino] == INFINITO) {
        printf("No existe un camino de %s a %s.\n", inicio, destino);
    } else {
        printf("El combustible gastado de %s a %s es: %d\n", inicio, destino, distancias[idxDestino]);

        // Array para almacenar el camino
        Galaxia* caminoArray[100];
        int caminoLength = 0;

        // Reconstruir el camino desde el destino hasta el inicio
        Galaxia* camino = nodos[idxDestino];
        int idxCamino = idxDestino;
        while (camino != NULL) {
            caminoArray[caminoLength++] = camino;
            if (strcmp(camino->nombre, inicio) == 0) {
                break;
            }
            // Encontrar el índice de la galaxia actual
            idxCamino = -1;
            for (int i = 0; i < num_nodos; i++) {
                if (nodos[i] == camino) {
                    idxCamino = i;
                    break;
                }
            }
            if (idxCamino == -1) {
                printf("Error al reconstruir el camino.\n");
                return;
            }
            camino = predecesores[idxCamino];
        }

        // Verificar que el camino sea válido
        if (camino == NULL || strcmp(camino->nombre, inicio) != 0) {
            printf("No se pudo encontrar un camino válido de %s a %s.\n", inicio, destino);
            return;
        }

        // Simular el viaje y el consumo de combustible con restricción de reabastecimiento
        printf("El camino más corto es: ");
        int combustibleRestante = combustible;
        for (int i = caminoLength - 1; i >= 0; i--) {
            printf("%s", caminoArray[i]->nombre);

            if (i > 0) {
                // Encontrar el peso de la arista entre el nodo actual y el siguiente en el camino
                int pesoArista = obtenerPesoArista(caminoArray[i], caminoArray[i - 1]->nombre);
                if (pesoArista == -1) {
                    printf("\nError al encontrar la arista entre %s y %s.\n", caminoArray[i]->nombre, caminoArray[i - 1]->nombre);
                    return;
                }

                // Verificar si hay suficiente combustible
                if (combustibleRestante < pesoArista) {
                    // Verificar si la galaxia actual es una estación de reabastecimiento
                    if (esEstacionReabastecimiento(caminoArray[i]->nombre)) {
                        // Reabastecer combustible en la galaxia actual
                        combustibleRestante = MAX_COMBUSTIBLE;
                        printf(" (Reabastecido en %s)", caminoArray[i]->nombre);
                    } else {
                        printf("\nNo es posible reabastecer en la galaxia '%s'. La nave se ha quedado sin combustible y ha quedado a la deriva. La tripulación ha muerto.\n", caminoArray[i]->nombre);
                        return;
                    }
                }

                // Restar el peso al combustible
                combustibleRestante -= pesoArista;

                // Verificar si el combustible se ha agotado
                if (combustibleRestante < 0) {
                    printf("\nLa nave se ha quedado sin combustible y ha quedado a la deriva. La tripulación ha muerto.\n");
                    return;
                }

                printf(" (combustible restante: %d) -> ", combustibleRestante);
            } else {
                printf("\n");
            }
        }
        printf("Combustible restante después del viaje: %d\n", combustibleRestante);

        // Actualizar la ubicación de la nave y el combustible global
        ubicacion_nave = strdup(destino);
        combustible = combustibleRestante;
    }
}

// Implementación de la búsqueda de camino más corto por número de galaxias (BFS) con consumo de combustible y restricción de reabastecimiento
void shortestPathByGalaxies(Galaxia* lista, char* inicio, char* destino) {
    Galaxia* nodos[100];
    int visitados[100] = {0};
    Galaxia* predecesores[100] = {NULL};
    int index = 0;

    // Mapear las galaxias a índices para facilitar el acceso
    Galaxia* actual = lista;
    while (actual != NULL) {
        nodos[index] = actual;
        actual = actual->siguiente;
        index++;
    }

    int num_nodos = index;

    // Encontrar los índices de inicio y destino
    int idxInicio = -1;
    int idxDestino = -1;
    for (int i = 0; i < num_nodos; i++) {
        if (strcmp(nodos[i]->nombre, inicio) == 0) {
            idxInicio = i;
        }
        if (strcmp(nodos[i]->nombre, destino) == 0) {
            idxDestino = i;
        }
    }

    if (idxInicio == -1) {
        printf("Error: La galaxia de inicio no existe\n");
        return;
    }
    if (idxDestino == -1) {
        printf("Error: La galaxia de destino no existe\n");
        return;
    }

    // Inicializar la cola para BFS
    int queue[100];
    int front = 0;
    int rear = 0;

    // Encolar el nodo de inicio
    queue[rear++] = idxInicio;
    visitados[idxInicio] = 1;

    // Bucle principal de BFS
    while (front < rear) {
        int idxActual = queue[front++];
        Galaxia* galaxiaActual = nodos[idxActual];

        // Verificar si hemos llegado al destino
        if (idxActual == idxDestino) {
            // Construir el camino utilizando los predecesores
            // Reconstruir el camino inversamente
            int path[100];
            int pathLength = 0;
            int idx = idxDestino;
            while (idx != idxInicio) {
                path[pathLength++] = idx;
                // Obtener el índice del predecesor
                for (int i = 0; i < num_nodos; i++) {
                    if (nodos[i] == predecesores[idx]) {
                        idx = i;
                        break;
                    }
                }
            }
            path[pathLength++] = idxInicio;

            // Simular el viaje y el consumo de combustible con restricción de reabastecimiento
            printf("El camino más corto en términos de número de galaxias de %s a %s es:\n", inicio, destino);
            int combustibleRestante = combustible;
            for (int i = pathLength - 1; i >= 0; i--) {
                printf("%s", nodos[path[i]]->nombre);
                if (i > 0) {
                    // Obtener el peso de la arista entre nodos[path[i]] y nodos[path[i - 1]]
                    int pesoArista = obtenerPesoArista(nodos[path[i]], nodos[path[i - 1]]->nombre);
                    if (pesoArista == -1) {
                        printf("\nError al encontrar la arista entre %s y %s.\n", nodos[path[i]]->nombre, nodos[path[i - 1]]->nombre);
                        return;
                    }

                    // Verificar si hay suficiente combustible
                    if (combustibleRestante < pesoArista) {
                        // Verificar si la galaxia actual es una estación de reabastecimiento
                        if (esEstacionReabastecimiento(nodos[path[i]]->nombre)) {
                            // Reabastecer combustible en la galaxia actual
                            combustibleRestante = MAX_COMBUSTIBLE;
                            printf(" (Reabastecido en %s)", nodos[path[i]]->nombre);
                        } else {
                            printf("\nNo es posible reabastecer en la galaxia '%s'. La nave se ha quedado sin combustible y ha quedado a la deriva. La tripulación ha muerto.\n", nodos[path[i]]->nombre);
                            return;
                        }
                    }

                    // Restar el peso al combustible
                    combustibleRestante -= pesoArista;

                    // Verificar si el combustible se ha agotado
                    if (combustibleRestante < 0) {
                        printf("\nLa nave se ha quedado sin combustible y ha quedado a la deriva. La tripulación ha muerto.\n");
                        return;
                    }

                    printf(" (combustible restante: %d) -> ", combustibleRestante);
                } else {
                    printf("\n");
                }
            }
            printf("Número de galaxias en el camino: %d\n", pathLength);
            printf("Combustible restante después del viaje: %d\n", combustibleRestante);

            // Actualizar la ubicación de la nave y el combustible global
            ubicacion_nave = strdup(destino);
            combustible = combustibleRestante;

            return;
        }

        // Explorar las galaxias adyacentes
        Arista* arista = galaxiaActual->adyacencias;
        while (arista != NULL) {
            // Encontrar el índice de la galaxia adyacente
            int idxAdyacente = -1;
            for (int i = 0; i < num_nodos; i++) {
                if (strcmp(nodos[i]->nombre, arista->destino) == 0) {
                    idxAdyacente = i;
                    break;
                }
            }
            if (idxAdyacente != -1 && !visitados[idxAdyacente]) {
                visitados[idxAdyacente] = 1;
                predecesores[idxAdyacente] = galaxiaActual;
                queue[rear++] = idxAdyacente;
            }
            arista = arista->siguiente;
        }
    }

    // Si no se encontró un camino
    printf("No existe un camino de %s a %s.\n", inicio, destino);
}

// Función para mostrar galaxias vecinas en un radio dado
void mostrarGalaxiasVecinas(Galaxia* galaxia, int radio) {
    printf("Galaxias vecinas a '%s' en un radio de %d:\n", galaxia->nombre, radio);

    // Utilizaremos una búsqueda en anchura limitada por el radio
    typedef struct Nodo {
        Galaxia* galaxia;
        int nivel;
    } Nodo;

    Nodo cola[100];
    int visitados[100] = {0};
    int front = 0, rear = 0;

    // Mapear las galaxias a índices
    Galaxia* nodos[100];
    int index = 0;
    Galaxia* actual = galaxias;
    while (actual != NULL) {
        nodos[index++] = actual;
        actual = actual->siguiente;
    }
    int num_nodos = index;

    // Encontrar el índice de la galaxia actual
    int idxActual = -1;
    for (int i = 0; i < num_nodos; i++) {
        if (nodos[i] == galaxia) {
            idxActual = i;
            break;
        }
    }
    if (idxActual == -1) {
        printf("Error al encontrar la galaxia actual en la lista.\n");
        return;
    }

    // Iniciar la búsqueda
    visitados[idxActual] = 1;
    cola[rear++] = (Nodo){galaxia, 0};

    while (front < rear) {
        Nodo nodoActual = cola[front++];
        if (nodoActual.nivel >= radio) {
            continue;
        }

        Arista* arista = nodoActual.galaxia->adyacencias;
        while (arista != NULL) {
            // Encontrar la galaxia adyacente
            Galaxia* galaxiaAdyacente = buscarGalaxia(galaxias, arista->destino);

            // Encontrar el índice de la galaxia adyacente
            int idxAdyacente = -1;
            for (int i = 0; i < num_nodos; i++) {
                if (nodos[i] == galaxiaAdyacente) {
                    idxAdyacente = i;
                    break;
                }
            }
            if (idxAdyacente == -1) {
                arista = arista->siguiente;
                continue;
            }

            if (!visitados[idxAdyacente]) {
                visitados[idxAdyacente] = 1;
                printf("- %s (Distancia: %d)\n", galaxiaAdyacente->nombre, nodoActual.nivel + 1);
                cola[rear++] = (Nodo){galaxiaAdyacente, nodoActual.nivel + 1};
            }
            arista = arista->siguiente;
        }
    }
}

// Verificar si una galaxia es vecina de otra
int esVecina(Galaxia* galaxia, char* nombreDestino) {
    Arista* arista = galaxia->adyacencias;
    while (arista != NULL) {
        if (strcmp(arista->destino, nombreDestino) == 0) {
            return 1; // Es vecina
        }
        arista = arista->siguiente;
    }
    return 0; // No es vecina
}

// Obtener el peso de la arista entre dos galaxias
int obtenerPesoArista(Galaxia* origen, char* destino) {
    Arista* arista = origen->adyacencias;
    while (arista != NULL) {
        if (strcmp(arista->destino, destino) == 0) {
            return arista->peso;
        }
        arista = arista->siguiente;
    }
    return -1;
}

// Función para manejar la secuencia de viaje en modo guiado
void secuenciaDeViaje(Galaxia* galaxiaActual) {
    char secuencia[500];
    printf("Ingrese la secuencia de galaxias separadas por comas (por ejemplo: G1,G2,G3): ");
    scanf("%s", secuencia);

    char* token = strtok(secuencia, ",");
    while (token != NULL) {
        trimWhitespace(token);
        Galaxia* galaxiaDestino = buscarGalaxia(galaxias, token);
        if (galaxiaDestino == NULL) {
            printf("La galaxia '%s' no existe. Viaje cancelado.\n", token);
            return;
        }
        if (esVecina(galaxiaActual, token)) {
            // Obtener el peso de la arista
            int peso = obtenerPesoArista(galaxiaActual, token);

            // Verificar si hay suficiente combustible
            if (combustible < peso) {
                // Verificar si la galaxia actual es una estación de reabastecimiento
                if (esEstacionReabastecimiento(galaxiaActual->nombre)) {
                    printf("No hay suficiente combustible para viajar a '%s'.\n", token);
                    printf("¿Desea reabastecer combustible en '%s'? (s/n): ", galaxiaActual->nombre);
                    char respuesta;
                    scanf(" %c", &respuesta);
                    if (respuesta == 's' || respuesta == 'S') {
                        combustible = MAX_COMBUSTIBLE;
                        printf("Combustible reabastecido en '%s'. Combustible actual: %d\n", galaxiaActual->nombre, combustible);
                    } else {
                        printf("Viaje cancelado.\n");
                        return;
                    }
                } else {
                    printf("No es posible reabastecer en la galaxia '%s'. Viaje cancelado.\n", galaxiaActual->nombre);
                    return;
                }
            }

            // Restar el peso al combustible
            combustible -= peso;

            // Actualizar la galaxia actual
            galaxiaActual = galaxiaDestino;
            printf("Viaje exitoso a la galaxia '%s'. Combustible restante: %d\n", token, combustible);

            // Verificar si el combustible se ha agotado
            if (combustible <= 0) {
                printf("La nave se ha quedado sin combustible y ha quedado a la deriva. La tripulación ha muerto.\n");
                exit(0); // Finalizar el programa
            }
        } else {
            printf("La galaxia '%s' no es vecina de la galaxia actual '%s'. Viaje cancelado.\n", token, galaxiaActual->nombre);
            return;
        }
        token = strtok(NULL, ",");
    }
    // Actualizar la ubicación de la nave
    ubicacion_nave = strdup(galaxiaActual->nombre);
}

// Función principal para manejar el viaje guiado
void viajeGuiado() {
    char opcion[10];
    Galaxia* galaxiaActual = buscarGalaxia(galaxias, ubicacion_nave);
    if (galaxiaActual == NULL) {
        printf("Error: La ubicación actual de la nave no es válida.\n");
        return;
    }

    while (1) {
        printf("\nUbicación actual: %s\n", galaxiaActual->nombre);
        printf("Combustible restante: %d\n", combustible);
        printf("Opciones:\n");
        printf("1. Mostrar galaxias vecinas\n");
        printf("2. Viajar a una galaxia vecina\n");
        printf("3. Definir una secuencia de galaxias para viajar\n");
        printf("4. Reabastecer combustible\n");
        printf("5. Salir\n");
        printf("Ingrese su opción: ");
        scanf("%s", opcion);

        if (strcmp(opcion, "1") == 0) {
            int radio;
            printf("Ingrese el radio (número entero positivo): ");
            scanf("%d", &radio);
            if (radio <= 0) {
                printf("El radio debe ser un número entero positivo.\n");
                continue;
            }
            mostrarGalaxiasVecinas(galaxiaActual, radio);
        } else if (strcmp(opcion, "2") == 0) {
            char destino[100];
            printf("Ingrese el nombre de la galaxia vecina a la que desea viajar: ");
            scanf("%s", destino);
            Galaxia* galaxiaDestino = buscarGalaxia(galaxias, destino);
            if (galaxiaDestino == NULL) {
                printf("La galaxia '%s' no existe.\n", destino);
                continue;
            }
            if (esVecina(galaxiaActual, destino)) {
                // Obtener el peso de la arista
                int peso = obtenerPesoArista(galaxiaActual, destino);

                // Verificar si hay suficiente combustible
                if (combustible < peso) {
                    // Verificar si la galaxia actual es una estación de reabastecimiento
                    if (esEstacionReabastecimiento(galaxiaActual->nombre)) {
                        printf("No hay suficiente combustible para realizar el viaje.\n");
                        printf("¿Desea reabastecer combustible en '%s'? (s/n): ", galaxiaActual->nombre);
                        char respuesta;
                        scanf(" %c", &respuesta);
                        if (respuesta == 's' || respuesta == 'S') {
                            combustible = MAX_COMBUSTIBLE;
                            printf("Combustible reabastecido. Combustible actual: %d\n", combustible);
                        } else {
                            printf("Viaje cancelado.\n");
                            continue;
                        }
                    } else {
                        printf("No es posible reabastecer en la galaxia '%s'.\n", galaxiaActual->nombre);
                        continue;
                    }
                }

                // Restar el peso al combustible
                combustible -= peso;

                // Actualizar la galaxia actual
                galaxiaActual = galaxiaDestino;
                printf("Viaje exitoso a la galaxia '%s'. Combustible restante: %d\n", destino, combustible);

                // Verificar si el combustible se ha agotado
                if (combustible <= 0) {
                    printf("La nave se ha quedado sin combustible y ha quedado a la deriva. La tripulación ha muerto.\n");
                    exit(0); // Finalizar el programa
                }
            } else {
                printf("La galaxia '%s' no es vecina de la galaxia actual '%s'.\n", destino, galaxiaActual->nombre);
            }
        } else if (strcmp(opcion, "3") == 0) {
            secuenciaDeViaje(galaxiaActual);
        } else if (strcmp(opcion, "4") == 0) {
            // Verificar si la galaxia actual es una estación de reabastecimiento
            if (esEstacionReabastecimiento(galaxiaActual->nombre)) {
                // Reabastecer combustible en la galaxia actual
                combustible = MAX_COMBUSTIBLE;
                printf("Combustible reabastecido en '%s'. Combustible actual: %d\n", galaxiaActual->nombre, combustible);
            } else {
                printf("No es posible reabastecer en la galaxia '%s'.\n", galaxiaActual->nombre);
            }
        } else if (strcmp(opcion, "5") == 0) {
            printf("Finalizando viaje guiado.\n");
            break;
        } else {
            printf("Opción inválida.\n");
        }
    }

    // Actualizar la ubicación de la nave al finalizar
    ubicacion_nave = strdup(galaxiaActual->nombre);
}

// Función principal
int main() {
    // Cargar los datos desde el archivo generado
    cargarDatos("../Parte1/salida.txt");

    // Imprimir el estado de la nave
    printf("La nave esta en %s con %d unidades de combustible.\n", ubicacion_nave, combustible);

    // Preguntar al usuario el modo de viaje
    printf("Seleccione el modo de viaje:\n");
    printf("1. Autonomo\n");
    printf("2. Guiado\n");
    printf("Ingrese su opcion (1 o 2): ");
    int opcion_modo;
    scanf("%d", &opcion_modo);

    if (opcion_modo == 1) {
        modo_viaje = 0; // Autónomo
    } else if (opcion_modo == 2) {
        modo_viaje = 1; // Guiado
    } else {
        printf("Opción inválida. Se usará el modo autónomo por defecto.\n");
        modo_viaje = 0;
    }

    if (modo_viaje == 0) {
        // Modo autónomo
        char destino[100];
        printf("Ingrese el destino para la nave: ");
        scanf("%s", destino);

        // Preguntar al usuario qué algoritmo desea usar
        int opcion;
        printf("Seleccione el tipo de ruta a calcular:\n");
        printf("1. Ruta que gasta menos combustible\n");
        printf("2. Ruta con menor número de galaxias\n");
        printf("Ingrese su opción (1 o 2): ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            dijkstraConCombustible(galaxias, ubicacion_nave, destino);
        } else if (opcion == 2) {
            shortestPathByGalaxies(galaxias, ubicacion_nave, destino);
        } else {
            printf("Opción inválida.\n");
        }
    } else {
        // Modo guiado
        viajeGuiado();
    }

    return 0;
}
