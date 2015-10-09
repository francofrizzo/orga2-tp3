/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"


#define MAX_CANT_PERROS_VIVOS             8
#define ESCONDITES_CANTIDAD               8
#define MAX_SIN_CAMBIOS                 999


#define JUGADOR_A                         0
#define JUGADOR_B                         1

#define MAPA_ANCHO                       80
#define MAPA_ALTO                        44

#define TIPO_1                            0
#define TIPO_2                            1


typedef enum direccion_e { ARR = 0x4, ABA = 0x7, DER = 0xA, IZQ = 0xD, AQUI = 0x10} direccion;

struct jugador_t;

typedef struct perro_t
{

	// ~~~ para ser completado ~~~
    uint index;  // indice de 0 a 15
    struct jugador_t *jugador;

	uint id;     // id unica tarea
	uint tipo;   // raza del perro
    uint libre;  // libre: -true- slot disponible para lanzar perro  / -false- ya hay un perro vivo

    uint x;
    uint y;

	uint huesos;
    uint indice_reloj;

} perro_t;


extern int escondites[ESCONDITES_CANTIDAD][3];
extern int ultimo_cambio;
extern perro_t *game_perro_actual;


typedef struct jugador_t
{
    uint index;    // 0 o 1
    perro_t perros[MAX_CANT_PERROS_VIVOS];   // los perros del jugador
    uint x_cucha, y_cucha;

    // posicion, puntos, ordenes, etc.
	// completar si es necesario.
	int x, y;  // posicion
    uint puntos;

} jugador_t;

extern jugador_t jugadorA, jugadorB;

/*
================================================================================
// ~~~ auxiliares dadas, sugeridas o requeridas (segun disponga enunciado) ~~~
================================================================================
*/

void ASSERT_OR_ERROR(uint value, char* error_msg);


// transforma posiciones de la forma (x,y) a algo lineal, de la forma x*ancho+y
uint game_xy2lineal();

// devuelve si la posicion dada es valida o no
uint game_es_posicion_valida(int x, int y);

/*
================================================================================
                          ~~~ auxiliares de perros ~~~
================================================================================
*/

// realiza inicialización básica de un perro. El perro aun no está vivo ni por lanzarse. Setea jugador, indice, etc
void game_perro_inicializar(perro_t *perro, jugador_t *jugador, uint index, uint id);

// toma un perro ya existente libre y lo recicla seteando x e y a la cucha.
// luego inicializa su mapeo de memoria, tss, lo agrega al sched y pinta la pantalla
void game_perro_reciclar_y_lanzar(perro_t *perro, uint tipo);

// el perro descargó sus huesos o realizó una acción no válida y caputó, hay que sacarlo del sistema.
void game_perro_termino(perro_t *perro);

// recibe una direccion y un perro, al cual debe mover en esa dirección
// *** viene del syscall mover ***
uint game_perro_mover(perro_t *perro, direccion dir);

// recibe un perro, el cual debe cavar en su posición
// *** viene del syscall cavar ***
uint game_perro_cavar(perro_t *perro);

// recibe un perro, devueve la dirección del hueso más cercano
// *** viene del syscall olfatear ***
uint game_perro_olfatear(perro_t *perro);

// chequea si el perro está en la cucha y suma punto al jugador o lo manda a dormir
void game_perro_ver_si_en_cucha(perro_t *perro);

/*
================================================================================
                         ~~~ auxiliares de jugadores ~~~
================================================================================
*/

// inicialización básica
void game_jugador_inicializar(jugador_t *j);

// debe encargarse de buscar un perro libre y lanzarlo
void game_jugador_lanzar_perro(jugador_t *j, uint tipo, int x, int y);

// recibe un par (x, y) y un jugador, al cual debe mover en esa dirección
uint game_jugador_moverse(jugador_t *j, int x, int y);

// descarga 1 hueso en la cucha y actualiza el screen
void game_jugador_anotar_punto(jugador_t *j);

// guarda la orden en el jugador para que los perros puedan preguntarla luego (mediante un syscall)
void game_jugador_dar_orden(jugador_t *jugador, int orden);


/*
================================================================================
                         ~~~ auxiliares del juego ~~~
================================================================================
*/

// debe atender la interrupción de reloj para actualizar la pantalla y terminar
// si es hora, recibe el perro que está corriendo actualmente
void game_atender_tick(perro_t *perro);

// devuelve la cantidad de huesos que hay en la posición pasada como parametro
uint game_huesos_en_posicion(uint x, uint y);

// devuelve algun perro que esté en la posicion pasada (hay max 2, uno por jugador)
perro_t* game_perro_en_posicion(uint x, uint y);

// termina si se agotaron los huesos o si hace tiempo que no hay ningun cambio
void game_terminar_si_es_hora();

#endif  /* !__GAME_H__ */
