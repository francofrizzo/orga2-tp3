/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "i386.h"

typedef struct extra_t {
	char mapa[80*50];
	int next_id;
	int tesoros[8][2];
	int next_id_minero;
	
} extra_t;


void task(int x_target, int y_target) {
    /* Tarea */

/*	extra_t *extra = (extra_t*)0x7FF000;

	int id = extra->next_id_minero++;
	if (extra->next_id_minero == 8) extra->next_id_minero = 0;

	if (x_target == 0)
	{
		int ii = 0;
		for (ii = 0; ii < 8; ii++)
			if (extra->tesoros[7-ii] != 0)
			{
				x_target = extra->tesoros[7-ii];
				y_target = extra->tesoros[7-ii];
				break;
			}
		}
	}*/

 /*   //breakpoint();
    int posicion = syscall_posicion(-1);
	int pos_x = posicion & 0xFF;
	int pos_y = (posicion >> 8) & 0xFF;


	char lado;
	if (pos_x == 1)
		lado = 0;
	else
		lado = 1;


	char avance = lado == 0? DER : IZQ;
	char retroc = lado == 0? IZQ : DER;
	char izq    = lado == 0? ARR : ABA;
	char der    = lado == 0? ABA : ARR;

    //breakpoint();
	if (lado == 0) {
		int i;
		for(i=pos_x; i < x_target; i++)
		    syscall_mover(avance);

		for(i=pos_y; i < y_target; i++)
		    syscall_mover(der);
	}
	else {
		int i;
		for(i=pos_x; i > x_target; i--)
		    syscall_mover(avance);

		for(i=pos_y; i > y_target; i--)
		    syscall_mover(der);
	}
	
    while(1) {syscall_cavar();}
*/
  while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
