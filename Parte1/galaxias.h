#ifndef GALAXIA_H
#define GALAXIA_H

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

extern Galaxia* galaxias;  
extern int combustible;    
extern char* ubicacion_nave;
extern char* nombre_nave;  // Añadido para almacenar el nombre de la nave

Galaxia* agregarGalaxia(Galaxia* lista, char* nombre);
Galaxia* buscarGalaxia(Galaxia* lista, char* nombre);
void agregarArista(Galaxia* galaxia, char* destino, int peso);
void modificarPesoArista(Galaxia* origen, char* destino, int nuevoPeso);
void guardarDatos(const char* nombreArchivo);

#endif 
