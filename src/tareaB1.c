/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "syscall.h"

// ejemplo de uso de memoria compartida para la guerra de tareas:
// parte (1)
// declaramos un stuct que va a guardar datos en 0x401000
//typedef struct extra_t {
// las variables de instancia pueden ser lo que queramos
//	char mapa[80*50];
//	int next_id;
//	int huesos[8][2];
//	
//} extra_t;


void task(int x, int y) {
    /* Tarea */

// parte (2)
//	extra_t *extra = (extra_t*)0x401000;
// a partir de aqui tenemos la estructura disponible 


    while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
