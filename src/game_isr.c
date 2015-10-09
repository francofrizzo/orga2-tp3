
#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"

/// ************** RUTINAS DE ATENCION DE INTERRUPCIONES DE ************** ///
///                              ALTO NIVEL                                ///
///                  (deben ser llamadas desde isr.asm)                    ///
/// ********************************************************************** ///


// gasta un rato en un ciclo infinito, util para hacer pausas y debuguear
void wait(int pseudosecs)
{
	int count;
	for (count = 0; count < pseudosecs * 1000000; count++) {}
}


uint game_syscall_manejar(uint syscall, uint param1)
{
    // ~ completar llamando a las funciones que haga falta ~
    return 0;
}

// ~~~ debe atender la interrupción de reloj para actualizar la pantalla y terminar si es hora,
// ~~~ recibe el perro que está corriendo actualmente
void game_atender_tick(perro_t *perro)
{
}


#define KB_w        0x11 // 0x91
#define KB_s        0x1f // 0x9f
#define KB_a        0x1e // 0x9e
#define KB_d        0x20 // 0xa0

#define KB_i        0x17 // 0x97
#define KB_k        0x25 // 0xa5
#define KB_j        0x24 // 0xa4
#define KB_l        0x26 // 0xa6


#define KB_q        0x10 // 0x90
#define KB_e        0x12 // 0x92

#define KB_u        0x16 // 0x96
#define KB_o        0x18 // 0x98

#define KB_z        0x2C // ?
#define KB_x        0x2D // ?
#define KB_c        0x2E // ?

#define KB_b        0x30 // ?
#define KB_n        0x31 // ?
#define KB_m        0x32 // ?

#define KB_shiftL   0x2a // 0xaa
#define KB_shiftR   0x36 // 0xb6


// ~~~ debe atender la interrupción de teclado, se le pasa la tecla presionada
void game_atender_teclado(unsigned char tecla)
{
	switch (tecla)
	{
		// ~~~ completar ~~~
		case KB_q: game_jugador_lanzar_perro(&jugadorA, TIPO_1, 0, 0); break;

		case KB_a: game_jugador_moverse(&jugadorA, -1,  0); break;


		case KB_k: game_jugador_moverse(&jugadorB,  0, -1); break;

		case KB_z: game_jugador_dar_orden(&jugadorA, 0); break;
		case KB_x: game_jugador_dar_orden(&jugadorA, 1); break;
		case KB_c: game_jugador_dar_orden(&jugadorA, 2); break;

		case KB_b: game_jugador_dar_orden(&jugadorB, 0); break;
		case KB_n: game_jugador_dar_orden(&jugadorB, 1); break;
		case KB_m: game_jugador_dar_orden(&jugadorB, 2); break;
		default: break;
	}

}

