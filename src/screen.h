/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */


#include "colors.h"
#include "defines.h"
#include "i386.h"
#include "sched.h"
#include "game.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;


struct perro_t;
typedef struct perro_t perro_t;

struct jugador_t;
typedef struct jugador_t jugador_t;

int ee_printf(const char *fmt, ...);

// pinta un "pixel" de la pantalla
void screen_pintar(unsigned char c, unsigned char color, uint fila, uint columna);

// imprime un string en pantalla
void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

// imprime un numero en hexa en pantalla
void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);


// pinta un rectangulo en pantalla
void screen_pintar_rect(unsigned char c, unsigned char color, int fila, int columna, int alto, int ancho);

// pinta una linea horizontal
void screen_pintar_linea_h(unsigned char c, unsigned char color, int fila, int columna, int ancho);

// pinta una linea vertical
void screen_pintar_linea_v(unsigned char c, unsigned char color, int fila, int columna, int alto);

// pinta el mapa, los huesos, los jugadores, etc
void screen_inicializar();

// pinta los puntajes en el rectángulo de abajo
void screen_pintar_puntajes();

// tick del reloj global
void screen_actualizar_reloj_global();

// tick del reloj de un perro
void screen_actualizar_reloj_perro (perro_t *perro);

// helper: color de un jugador
unsigned char screen_color_jugador(jugador_t *j);

// helper: caracter segun tipo de perro
unsigned char screen_caracter_perro(unsigned int tipo);

void screen_pintar_perro(perro_t *perro);
void screen_borrar_perro(perro_t *perro);
void screen_pintar_jugador(jugador_t *j);
void screen_borrar_jugador(jugador_t *j);

void screen_pintar_reloj_perro(perro_t *perro);
void screen_pintar_reloj_perros(jugador_t *j);
void screen_pintar_relojes();

void screen_actualizar_posicion_mapa(uint x, uint y);
void screen_stop_game_show_winner(jugador_t *j);


#endif  /* !__SCREEN_H__ */
