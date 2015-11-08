/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"

#include <stdarg.h>


int escondites[ESCONDITES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, HUESOS)
                                        {76,  25, 50}, {12, 15, 50}, {9, 10, 100}, {47, 21, 100} ,
                                        {34,  11, 50}, {75, 38, 50}, {40, 21, 100}, {72, 17, 100}
                                    };

jugador_t jugadorA;
jugador_t jugadorB;

perro_t *game_perro_actual = NULL;
int ultimo_cambio = MAX_SIN_CAMBIOS;

void ASSERT_OR_ERROR(uint value, char* error_msg)
{
	if (!value) {
		print(error_msg, 5, 5, C_BG_LIGHT_GREY | C_FG_BLACK);
		breakpoint();
	}
}

void* error()
{
	__asm__ ("int3");
	return 0;
}

uint game_xy2lineal (uint x, uint y) {
	return (y * MAPA_ANCHO + x);
}

uint game_es_posicion_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < MAPA_ANCHO && y < MAPA_ALTO);
}

// transforma código de dirección en valores x e y 
uint game_dir2xy(/* in */ direccion dir, /* out */ int *x, /* out */ int *y)
{
	switch (dir)
	{
		case IZQ: *x = -1; *y =  0; break;
		case DER: *x =  1; *y =  0; break;
		case ABA: *x =  0; *y =  1; break;
		case ARR: *x =  0; *y = -1; break;
    	default: return -1;
	}

	return 0;
}

void game_inicializar()
{
	game_jugador_inicializar(&jugadorA);
	game_jugador_inicializar(&jugadorB);

    screen_pintar_puntajes();
}

// devuelve la cantidad de huesos que hay en la posicion pasada como parametro
uint game_huesos_en_posicion(uint x, uint y)
{
	int i;
	for (i = 0; i < ESCONDITES_CANTIDAD; i++)
	{
		if (escondites[i][0] == x && escondites[i][1] == y)
			return escondites[i][2];
	}
	return 0;
}

// disminuye en uno la cantiddad de huesos en la posicion pasada como parametro
void game_dec_huesos(uint x, uint y)
{
	int i;
	for (i = 0; i < ESCONDITES_CANTIDAD; i++)
	{
		if (escondites[i][0] == x && escondites[i][1] == y && escondites[i][2] > 0)
			escondites[i][2]--;
	}
}

// devuelve algun perro que esté en la posicion pasada (hay max 2, uno por jugador)
perro_t* game_perro_en_posicion(uint x, uint y)
{
	int i;
	for (i = 0; i < MAX_CANT_PERROS_VIVOS; i++)
	{
		if (!jugadorA.perros[i].libre && jugadorA.perros[i].x == x && jugadorA.perros[i].y == y)
			return &jugadorA.perros[i];
		if (!jugadorB.perros[i].libre && jugadorB.perros[i].x == x && jugadorB.perros[i].y == y)
			return &jugadorB.perros[i];
	}
	return NULL;
}

// termina si se agotaron los huesos o si hace tiempo que no hay ningun cambio
void game_terminar_si_es_hora()
{
}

// se fija si la posicion dada contiene algun perro del jugador pasado por parametro
uint game_hay_perros_de(uint x, uint y, jugador_t* jugador) {
	uint hay_perros = FALSE;
	uint i;
	for (i = 0; i < MAX_CANT_PERROS_VIVOS; i++) {
		if (!jugador->perros[i].libre && jugador->perros[i].x == x && jugador->perros[i].y == y) {
			hay_perros = TRUE;
		}
	}
	return hay_perros;
}
