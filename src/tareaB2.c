/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "syscall.h"

void task(int x_target, int y_target) {
    /* Tarea */
  while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
