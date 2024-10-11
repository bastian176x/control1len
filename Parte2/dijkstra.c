#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  
#include "../Parte1/galaxias.h"
#include "../utils/utils.h"
 

#define INFINITO 999999


// Funciones relacionadas con el algoritmo de Dijkstra (puedes reutilizar las otras desde galaxia.h)
Galaxia* encontrarMenorDistancia(Galaxia* lista, int* distancias, int* visitados);
void dijkstra(Galaxia* lista, char* inicio, char* destino);
void cargarDatos(const char* nombreArchivo);
void viajeGuiado();
void mostrarGalaxiasVecinas(Galaxia* galaxia, int radio);
int esVecina(Galaxia* galaxia, char* nombreDestino);
int obtenerPesoArista(Galaxia* origen, char* destino);
void secuenciaDeViaje(Galaxia* galaxiaActual);


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
            printf("El camino más corto en términos de número de galaxias de %s a %s es:\n", inicio, destino);

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

            // Imprimir el camino en orden correcto
            for (int i = pathLength - 1; i >= 0; i--) {
                printf("%s", nodos[path[i]]->nombre);
                if (i > 0) {
                    printf(" -> ");
                }
            }
            printf("\n");

            // Imprimir el número de galaxias en el camino
            printf("Número de galaxias en el camino: %d\n", pathLength);

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
            sscanf(linea, "nave %[^,], combustible = %d, %[^,], reabastecer;", nombreNave, &combustible, ubicacion);
            trimWhitespace(nombreNave);    
            trimWhitespace(ubicacion);     
            ubicacion_nave = strdup(ubicacion);
        } else if (strncmp(linea, "nave ", 5) == 0) {
                char nombreNave[100], ubicacion[100], modo[20];
                sscanf(linea, "nave %[^,], combustible = %d, %[^,], reabastecer, %s;", nombreNave, &combustible, ubicacion, modo);
                trimWhitespace(nombreNave);
                trimWhitespace(ubicacion);
                trimWhitespace(modo);
                ubicacion_nave = strdup(ubicacion);
                if (strcmp(modo, "autonomo") == 0) {
                    modo_viaje = 0;
                } else if (strcmp(modo, "guiado") == 0) {
                    modo_viaje = 1;
                } else {
                    printf("Error: Modo de viaje no reconocido. Se usará modo autónomo por defecto.\n");
                    modo_viaje = 0;
                }
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

        Arista* arista = actual->adyacencias;
        while (arista != NULL) {
            int idxDestinoArista = -1;
            for (int i = 0; i < num_nodos; i++) {
                if (strcmp(nodos[i]->nombre, arista->destino) == 0) {
                    idxDestinoArista = i;
                    break;
                }
            }

            if (idxDestinoArista != -1 && distancias[idxActual] + arista->peso < distancias[idxDestinoArista]) {
                distancias[idxDestinoArista] = distancias[idxActual] + arista->peso;
                predecesores[idxDestinoArista] = actual;
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
        while (camino != NULL) {
            caminoArray[caminoLength++] = camino;
            if (camino == nodos[idxInicio]) {
                break;
            }
            // Encontrar el índice de la galaxia actual
            int idxCamino = -1;
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
        if (camino != nodos[idxInicio]) {
            printf("No se pudo encontrar un camino válido de %s a %s.\n", inicio, destino);
            return;
        }

        // Imprimir el camino en orden correcto y actualizar el combustible
        printf("El camino mas corto es: ");
        int combustibleRestante = combustible;
        for (int i = caminoLength - 1; i >= 0; i--) {
            printf("%s", caminoArray[i]->nombre);

            if (i > 0) {
                // Encontrar la arista entre el nodo actual y el siguiente en el camino
                Arista* arista = caminoArray[i]->adyacencias;
                int pesoArista = -1;
                while (arista != NULL) {
                    if (strcmp(arista->destino, caminoArray[i - 1]->nombre) == 0) {
                        pesoArista = arista->peso;
                        break;
                    }
                    arista = arista->siguiente;
                }
                if (pesoArista == -1) {
                    printf("\nError al encontrar la arista entre %s y %s.\n", caminoArray[i]->nombre, caminoArray[i - 1]->nombre);
                    return;
                }

                // Restar el peso al combustible
                combustibleRestante -= pesoArista;
                if (combustibleRestante <= 0) {
                    printf("\nLa nave se ha quedado sin combustible y no puede continuar.\n");
                    return;
                }

                printf(" (combustible restante: %d) -> ", combustibleRestante);
            } else {
                printf("\n");
            }
        }
        printf("Combustible restante despues del viaje: %d\n", combustibleRestante);

    
    }
}


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
            // Restar el combustible necesario
            int peso = obtenerPesoArista(galaxiaActual, token);
            if (combustible >= peso) {
                combustible -= peso;
                galaxiaActual = galaxiaDestino;
                printf("Viaje exitoso a la galaxia '%s'. Combustible restante: %d\n", token, combustible);
            } else {
                printf("No hay suficiente combustible para viajar a '%s'. Viaje cancelado.\n", token);
                return;
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

void viajeGuiado() {
    char opcion[10];
    Galaxia* galaxiaActual = buscarGalaxia(galaxias, ubicacion_nave);
    if (galaxiaActual == NULL) {
        printf("Error: La ubicación actual de la nave no es válida.\n");
        return;
    }

    while (1) {
        printf("\nUbicacion actual: %s\n", galaxiaActual->nombre);
        printf("Opciones:\n");
        printf("1. Mostrar galaxias vecinas\n");
        printf("2. Viajar a una galaxia vecina\n");
        printf("3. Definir una secuencia de galaxias para viajar\n");
        printf("4. Salir\n");
        printf("Ingrese su opcion: ");
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
                // Restar el combustible necesario
                int peso = obtenerPesoArista(galaxiaActual, destino);
                if (combustible >= peso) {
                    combustible -= peso;
                    galaxiaActual = galaxiaDestino;
                    printf("Viaje exitoso a la galaxia '%s'. Combustible restante: %d\n", destino, combustible);
                } else {
                    printf("No hay suficiente combustible para realizar el viaje.\n");
                }
            } else {
                printf("La galaxia '%s' no es vecina de la galaxia actual '%s'.\n", destino, galaxiaActual->nombre);
            }
        } else if (strcmp(opcion, "3") == 0) {
            secuenciaDeViaje(galaxiaActual);
        } else if (strcmp(opcion, "4") == 0) {
            printf("Finalizando viaje guiado.\n");
            break;
        } else {
            printf("Opcion inválida.\n");
        }
    }
}


int main() {
    // Cargar los datos desde el archivo generado
    cargarDatos("../Parte1/salida.txt");

    // Imprimir el estado de la nave
    printf("La nave está en %s con %d unidades de combustible.\n", ubicacion_nave, combustible);

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
        printf("Opción invalida. Se usará el modo autónomo por defecto.\n");
        modo_viaje = 0;
    }

    if (modo_viaje == 0) {
        // Modo autónomo
        // Ejecutar el algoritmo de Dijkstra o BFS según elección del usuario
        char destino[100];
        printf("Ingrese el destino para la nave: ");
        scanf("%s", destino);

        // Preguntar al usuario qué algoritmo desea usar
        int opcion;
        printf("Seleccione el tipo de ruta a calcular:\n");
        printf("1. Ruta que gasta menos combustible\n");
        printf("2. Ruta con menor numero de galaxias\n");
        printf("Ingrese su opcion (1 o 2): ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            dijkstra(galaxias, ubicacion_nave, destino);
        } else if (opcion == 2) {
            shortestPathByGalaxies(galaxias, ubicacion_nave, destino);
        } else {
            printf("Opcion invalida.\n");
        }
    } else {
        // Modo guiado
        viajeGuiado();
    }

    

    return 0;
}

