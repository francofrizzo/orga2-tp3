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

tss tss_prueba;

void tss_inicializar() {
	tss_idle.cs     = SEG_CODE_KERNEL;
	tss_idle.ds     = SEG_DATA_KERNEL;
	tss_idle.ss     = SEG_DATA_KERNEL;
	tss_idle.eip    = TASK_IDLE_CODE_ADDR;
	tss_idle.ebp    = BASE_PILA_KERNEL;
	tss_idle.esp    = BASE_PILA_KERNEL;
	tss_idle.cr3    = BASE_PD_KERNEL & 0xFFFFF000;
	tss_idle.eflags = 0x202;
	tss_idle.iomap  = 0xFFFF;

	gdt[GDT_IDX_TSS_INICIAL].base_0_15 = (uint) &tss_inicial & 0x0000FFFF;
	gdt[GDT_IDX_TSS_INICIAL].base_23_16 = ((uint) &tss_inicial >> 16) & 0x000000FF;
	gdt[GDT_IDX_TSS_INICIAL].base_31_24 = ((uint) &tss_inicial >> 24) & 0x000000FF;
	gdt[GDT_IDX_TSS_INICIAL].limit_0_15 = TSS_SIZE & 0x0000FFFF;
	gdt[GDT_IDX_TSS_INICIAL].limit_16_19 = (TSS_SIZE >> 16) & 0x0000000F;

	gdt[GDT_IDX_TSS_IDLE].base_0_15 = (uint) &tss_idle & 0x0000FFFF;
	gdt[GDT_IDX_TSS_IDLE].base_23_16 = ((uint) &tss_idle >> 16) & 0x000000FF;
	gdt[GDT_IDX_TSS_IDLE].base_31_24 = ((uint) &tss_idle >> 24) & 0x000000FF;
	gdt[GDT_IDX_TSS_IDLE].limit_0_15 = TSS_SIZE & 0x0000FFFF;
	gdt[GDT_IDX_TSS_IDLE].limit_16_19 = (TSS_SIZE >> 16) & 0x0000000F;
}

void tss_inicializar_perro(tss* tss_perro, perro_t* perro) {
	tss_perro->cs     = SEG_CODE_USER;
	tss_perro->ds     = SEG_DATA_USER;
	tss_perro->ss     = SEG_DATA_USER;
	tss_perro->eip    = CODIGO_BASE;     
	tss_perro->ebp    = TASK_PILA_BASE;
	tss_perro->esp    = TASK_PILA_BASE;
	tss_perro->cr3    = mmu_inicializar_memoria_perro(perro, perro->jugador->index, perro->index);
	tss_perro->eflags = 0x202;
	tss_perro->esp0   = mmu_proxima_pagina_fisica_libre();
	tss_perro->ss0    = SEG_DATA_KERNEL;
	tss_perro->iomap  = 0xFFFF;
}

void tss_agregar_a_gdt(tss* tss_in, gdt_entry* gdt_in, int idx) {
	if (0 < idx && idx < GDT_COUNT) {
		gdt_in[idx] = (gdt_entry) {
	        (unsigned short)     TSS_SIZE              & 0x0000FFFF, /* limit[0:15]  */
	        (unsigned short)     (uint) tss_in         & 0x0000FFFF, /* base[0:15]   */
	        (unsigned char)     ((uint) tss_in >> 16)  & 0x000000FF, /* base[23:16]  */
	        (unsigned char)      GDT_TYPE_TSS,                       /* type         */
	        (unsigned char)      0x00,                               /* s            */
	        (unsigned char)      GDT_DPL_KERNEL,                                /* dpl          */
	        (unsigned char)      0x01,                               /* p            */
	        (unsigned char)     (TSS_SIZE      >> 16)  & 0x0000000F, /* limit[16:19] */
	        (unsigned char)      0x00,                               /* avl          */
	        (unsigned char)      0x00,                               /* l            */
	        (unsigned char)      0x01,                               /* db           */
	        (unsigned char)      0x00,                               /* g            */
	        (unsigned char)     ((uint) tss_in >> 24)  & 0x000000FF  /* base[31:24]  */
	    };
	}
}

void tss_perro_prueba() {
    perro_t tomi;
    tomi.x = 25;
    tomi.y = 15;
    tomi.index = TIPO_1;
    tomi.jugador = &jugadorA;

    tss_inicializar_perro(&tss_prueba, &tomi);

    tss_agregar_a_gdt(&tss_prueba, (gdt_entry*) &gdt, 22);
}
