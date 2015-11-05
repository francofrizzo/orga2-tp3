/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */

tss tss_inicial;
tss tss_idle;

tss tss_jugadorA[MAX_CANT_PERROS_VIVOS];
tss tss_jugadorB[MAX_CANT_PERROS_VIVOS];

void tss_inicializar() {
	tss_idle.cs     = SEG_CODE_KERNEL;
	tss_idle.ds     = SEG_DATA_KERNEL;
	tss_idle.ss     = SEG_DATA_KERNEL;
	tss_idle.eip    = TASK_IDLE_CODE_ADDR;
	tss_idle.ebp    = BASE_PILA_KERNEL;
	tss_idle.esp    = BASE_PILA_KERNEL;
	tss_idle.cr3    = BASE_PD_KERNEL & 0xFFFFF000;
	tss_idle.eflags = 0x202;

}

void completar_tss(tss* entrada_tss, perro_t* perro) {
	entrada_tss->cs     = SEG_CODE_USER;
	entrada_tss->ds     = SEG_DATA_USER;
	entrada_tss->ss     = SEG_DATA_USER;
	entrada_tss->eip    = CODIGO_BASE;     
	entrada_tss->ebp    = TASK_PILA_BASE;
	entrada_tss->esp    = TASK_PILA_BASE;
	entrada_tss->cr3    = mmu_inicializar_memoria_perro(perro, perro->jugador->index, perro->index);
	entrada_tss->eflags = 0x202;
	entrada_tss->esp0   = mmu_proxima_pagina_fisica_libre();
}
