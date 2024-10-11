#ifndef GALAXIA_H
#define GALAXIA_H

// Definición de las estructuras
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

// Variables globales
extern Galaxia* galaxias;  
extern int combustible;    
extern char* ubicacion_nave; 
extern int modo_viaje; 


// Declaración de las funciones
Galaxia* agregarGalaxia(Galaxia* lista, char* nombre);
Galaxia* buscarGalaxia(Galaxia* lista, char* nombre);
void agregarArista(Galaxia* galaxia, char* destino, int peso);
void guardarDatos(const char* nombreArchivo);
void modificarPesoArista(Galaxia* galaxias);

#endif 
