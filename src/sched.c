/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "screen.h"

sched_t scheduler;

void sched_inicializar() {
    scheduler.tasks[0].gdt_index = TSS_IDLE;
    scheduler.tasks[0].perro     = NULL;

    int i;
    for (i = 1; i < MAX_CANT_TAREAS_VIVAS; i++) {
        scheduler.tasks[i].gdt_index = NULL;
        scheduler.tasks[i].perro     = NULL;
    }

    scheduler.current         = NULL;
    scheduler.proximo_jugador = JUGADOR_A;
    scheduler.last_jugadorA   = NULL;
    scheduler.last_jugadorB   = NULL;
}

int sched_buscar_indice_tarea(uint gdt_index) {
    int i;
    for (i = 0; i < MAX_CANT_TAREAS_VIVAS + 1; i++) {
        if (scheduler.tasks[i].gdt_index == gdt_index) {
            return i;
        }
    }
    return -1;
}

int sched_buscar_tarea_libre() {
    return sched_buscar_indice_tarea(NULL);
}

perro_t* sched_tarea_actual() {
    return scheduler.tasks[scheduler.current].perro;
}

void sched_agregar_tarea(perro_t *perro) {
    int i = sched_buscar_tarea_libre();
    if (i != -1) {
        sched_task_t nueva_tarea = {
            TSS_PERRO(perro->jugador->index, perro->index),  // gdt_index
            perro                                            // perro
        };
        scheduler.tasks[i] = nueva_tarea;
    }
}

void sched_remover_tarea(unsigned int gdt_index) {
    int i = sched_buscar_indice_tarea(gdt_index);
    scheduler.tasks[i].gdt_index = NULL;
    scheduler.tasks[i].perro     = NULL;
}

uint sched_proxima_a_ejecutar() {
    uint last_prox_jugador =    scheduler.proximo_jugador == JUGADOR_A ?
                                    scheduler.last_jugadorA            :
                                    scheduler.last_jugadorB;
    uint i = last_prox_jugador;
    do {
        if (i < MAX_CANT_TAREAS_VIVAS) {
            i++;
        } else {
            i = 0;
        }
    } while ((scheduler.tasks[i].perro == NULL ||
        scheduler.tasks[i].perro->index != scheduler.proximo_jugador) &&
        i != last_prox_jugador);

    if (scheduler.tasks[i].perro->index != scheduler.proximo_jugador) {
        i = 0;
    }

    return i;
}

ushort sched_atender_tick() {
    if (!debug_screen) {
        game_atender_tick(scheduler.tasks[scheduler.current].perro);

        scheduler.current = sched_proxima_a_ejecutar();
        game_perro_actual = scheduler.tasks[scheduler.current].perro;

        if (scheduler.proximo_jugador == JUGADOR_A) {
            scheduler.last_jugadorA = scheduler.current;
            scheduler.proximo_jugador = JUGADOR_B;
        } else {
            scheduler.last_jugadorB = scheduler.current;
            scheduler.proximo_jugador = JUGADOR_A;
        }

        tlbflush(); // ES NECESARIO?
        return scheduler.tasks[scheduler.current].gdt_index;
    } else {
        return TSS_IDLE;
    }
}
