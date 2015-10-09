/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "game.h"

#define MAX_CANT_TAREAS_VIVAS   (MAX_CANT_PERROS_VIVOS * 2)

/*
================================================================================
                       ~~~ estructuras del scheduler ~~~
================================================================================
*/

// para el scheduler, lo importante de cada tarea es su indice en la gdt y
// el perro al cual está asociada. Si el indice es null significará que está libre.
typedef struct sched_task_t
{
    unsigned int gdt_index;
	perro_t *perro;
} sched_task_t;

// el scheduler posee un arreglo de tareas (cada una puede estar libre o asignada)
typedef struct sched_t
{
    sched_task_t tasks[MAX_CANT_TAREAS_VIVAS+1];
    ushort current;
} sched_t;

extern sched_t scheduler;

/*
================================================================================
                       ~~~ funciones del scheduler ~~~
================================================================================
*/

// debe agregar una tarea al scheduler, buscando un slot libre y asociando indice de gdt y perro 
void sched_agregar_tarea(perro_t *perro);

// debe remover tareas del scheduler
void sched_remover_tarea(unsigned int gdt_index);

// debe devolver el perro correspondiente a la tarea que está corriendo actualmente
perro_t* sched_tarea_actual();

// debe avisar al juego que hubo un tick (para que haga cosas del juego) y luego configurarse
// para pasar a la siguiente tarea (devuelve su indice en la gdt)
ushort sched_atender_tick();

#endif	/* !__SCHED_H__ */
