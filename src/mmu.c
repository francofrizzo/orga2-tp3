/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"


/* Atributos paginas */
/* -------------------------------------------------------------------------- */


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */


/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

#define BASE_PD_KERNEL		    0x27000
#define BASE_PT_KERNEL(pag)     (0x28000 + (pag << 12))


/* Implementacion de funciones del manejador de memoria */
/* -------------------------------------------------------------------------- */

void mmu_mapear_pagina (uint virtual, uint cr3, uint fisica, uint attrs) {
	uint pd_index = virtual >> 22;
	uint pt_index = (virtual >> 12) & 0x000003FF;
	
	uint* pd = (uint*) (cr3 & 0xFFFFF000);
	uint* pt = (uint*) (pd[pd_index] & 0xFFFFF000);

	pt[pt_index] = (fisica & 0xFFFFF000) | (attrs & 0x00000FFF);
}

uint mmu_inicializar_dir_kernel() {
	uint* pd_kernel = (uint*) (BASE_PD_KERNEL & 0xFFFFF000);

	int i;
	for (i = 0; i < 1024; i++) {  // limpiamos el directorio
		pd_kernel[i] = 0;
	}

	pd_kernel[0] = (BASE_PT_KERNEL(0) & 0xFFFFF000) | 0x3;  // seteamos la primera entrada del directorio, atributos: r/w, p

	for (i = 0; i < 1024; i++) {  // mapeamos con identity mapping las primeras 1024 paginas
		mmu_mapear_pagina(i << 12, (uint) pd_kernel, i << 12, 0x3);
	}

	return (uint) pd_kernel;
}
