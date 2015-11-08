
#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"

/// ************** RUTINAS DE ATENCION DE INTERRUPCIONES DE ************** ///
///                              ALTO NIVEL                                ///
///                  (deben ser llamadas desde isr.asm)                    ///
/// ********************************************************************** ///


/* Codigos de syscalls */
/* -------------------------------------------------------------------------- */
#define SYSCALL_MOVERSE           0x1
#define SYSCALL_CAVAR             0x2
#define SYSCALL_OLFATEAR          0x3
#define SYSCALL_RECIBIR_ORDEN     0x4

// gasta un rato en un ciclo infinito, util para hacer pausas y debuguear
void wait(int pseudosecs)
{
    int count;
    for (count = 0; count < pseudosecs * 1000000; count++) {}
}

uint game_syscall_manejar(uint syscall, uint param1)
{
    uint res = 0;
    switch (syscall) {
        case SYSCALL_MOVERSE: {
            res = game_perro_mover(game_perro_actual, param1);
            break;
        }
        case SYSCALL_CAVAR: {
            res = game_perro_cavar(game_perro_actual);
            break;
        }
        case SYSCALL_OLFATEAR: {
            res = game_perro_olfatear(game_perro_actual);
            break;
        }
        case SYSCALL_RECIBIR_ORDEN: {
            res = (game_perro_actual->jugador->ultima_orden) << 16 |
                (game_perro_actual->jugador->y) << 8 |
                (game_perro_actual->jugador->x);
            break;
        }
        default: {
            res = -1;
            break;
        }
    }
    return res;
}

// ~~~ debe atender la interrupción de reloj para actualizar la pantalla y terminar si es hora,
// ~~~ recibe el perro que está corriendo actualmente
void game_atender_tick(perro_t *perro) {
    screen_actualizar_reloj_global();
    if (perro != NULL) {
        game_perro_ver_si_en_cucha(perro);
        screen_actualizar_reloj_perro(perro);
    }
    // screen_show_debug_info();
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
    /* // Imprime en pantalla la tecla pulsada
    char tecla_ascii = 0;

    switch (tecla) {
        case KB_q: tecla_ascii = 'q'; break;
        case KB_e: tecla_ascii = 'e'; break;
        case KB_u: tecla_ascii = 'u'; break;
        case KB_o: tecla_ascii = 'o'; break;

        case KB_a: tecla_ascii = 'a'; break;
        case KB_d: tecla_ascii = 'd'; break;
        case KB_w: tecla_ascii = 'w'; break;
        case KB_s: tecla_ascii = 's'; break;

        case KB_j: tecla_ascii = 'j'; break;
        case KB_l: tecla_ascii = 'l'; break;
        case KB_i: tecla_ascii = 'i'; break;
        case KB_k: tecla_ascii = 'k'; break;

        case KB_z: tecla_ascii = 'z'; break;
        case KB_x: tecla_ascii = 'x'; break;
        case KB_c: tecla_ascii = 'c'; break;

        case KB_b: tecla_ascii = 'b'; break;
        case KB_n: tecla_ascii = 'n'; break;
        case KB_m: tecla_ascii = 'm'; break;
        default: break;
    }

    if (tecla_ascii != 0) {
        screen_pintar(tecla_ascii, C_FG_LIGHT_GREY | C_BG_BLACK, 0, VIDEO_COLS - 12);
    }
    */

    switch (tecla) {
        // Lanzar perros
        case KB_q: game_jugador_lanzar_perro(&jugadorA, TIPO_1, jugadorA.x_cucha, jugadorA.y_cucha); break; // jugador A, tipo 1
        case KB_e: game_jugador_lanzar_perro(&jugadorA, TIPO_2, jugadorA.x_cucha, jugadorA.y_cucha); break; // jugador A, tipo 2
        case KB_u: game_jugador_lanzar_perro(&jugadorB, TIPO_1, jugadorB.x_cucha, jugadorB.y_cucha); break; // jugador B, tipo 1
        case KB_o: game_jugador_lanzar_perro(&jugadorB, TIPO_2, jugadorB.x_cucha, jugadorB.y_cucha); break; // jugador B, tipo 2

        // Moverse (jugador A)
        case KB_a: game_jugador_moverse(&jugadorA, -1, 0); break; // izquierda
        case KB_d: game_jugador_moverse(&jugadorA, 1, 0); break; // derecha
        case KB_w: game_jugador_moverse(&jugadorA, 0, -1); break; // arriba
        case KB_s: game_jugador_moverse(&jugadorA, 0, 1); break; // abajo

        // Moverse (jugador B)
        case KB_j: game_jugador_moverse(&jugadorB, -1, 0); break; // izquierda
        case KB_l: game_jugador_moverse(&jugadorB, 1, 0); break; // derecha
        case KB_i: game_jugador_moverse(&jugadorB, 0, -1); break; // arriba
        case KB_k: game_jugador_moverse(&jugadorB, 0, 1); break; // abajo

        // Dar ordenes (jugador A)
        case KB_z: game_jugador_dar_orden(&jugadorA, 0); break;
        case KB_x: game_jugador_dar_orden(&jugadorA, 1); break;
        case KB_c: game_jugador_dar_orden(&jugadorA, 2); break;
        
        // Dar ordenes (jugador B)
        case KB_b: game_jugador_dar_orden(&jugadorB, 0); break;
        case KB_n: game_jugador_dar_orden(&jugadorB, 1); break;
        case KB_m: game_jugador_dar_orden(&jugadorB, 2); break;
        default: break;
    }
}
