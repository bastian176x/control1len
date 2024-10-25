# control1len

PARTE 1:
DIRIGIRSE AL DIRECTORORIO PARTE 1 Y EJECUTAR:
>make
>./galaxia archivo_entrada.txt

    PARA CREAR NAVE:
    crear_nave <nombre nave> <combustible> <galaxia origen>
    crear_nave ALMENDRA 150 I1;

    PARA MODIFICAR ARISTA:
    modificar_arista A, B = peso = 1; 

    PARA SALIR DEL PROGRAMA:
    salir; !! MUY IMPORTANTE SALIR, DE LO CONTRARIO NO SE GUARDAN LOS DATOS EN EL ARCHIVO DE SALIDA !!

PARTE 2:
DIRIGIRSE AL DIRECTORORIO PARTE 2 Y EJECUTAR:
>make
>./dijkstra
COMANDOS

    viajar_secuencia [galaxia1] [galaxia2] ... [galaxiaN]: Define una secuencia de galaxias para viajar.
    mostrar_vecinas [radio]: Muestra las galaxias vecinas dentro del radio dado.
    combustible: Muestra el combustible restante de la nave.
    reabastecer: Reabastece combustible si la galaxia actual lo permite.
    ruta_optima [origen] [destino]: Calcula la ruta que gasta menos combustible entre dos galaxias.
    ruta_corta [origen] [destino]: Calcula la ruta con menos galaxias entre dos galaxias
    galaxia_actual: Muestra la galaxia en la que se encuentra la nave
