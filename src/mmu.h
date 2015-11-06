/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "game.h"

#define SHARED_BASE       0x400000
#define CODIGO_BASE       0x401000
#define TASK_PILA_BASE    0x402000 - 12

#define MAPA_BASE_FISICA  0x500000
#define MAPA_BASE_VIRTUAL 0x800000

#define AREA_LIBRE_BASE   0x100000
#define AREA_LIBRE_LIM    0x3FFFFF


/* Atributos paginas */
/* -------------------------------------------------------------------------- */

#define PAGE_ATTR_USER             0x4
#define PAGE_ATTR_READ_WRITE       0x2
#define PAGE_ATTR_PRESENT          0x1

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

#define TASK_A1_CODE_ADDR           0x10000
#define TASK_A2_CODE_ADDR           0x11000
#define TASK_B1_CODE_ADDR           0x12000
#define TASK_B2_CODE_ADDR           0x13000
#define TASK_IDLE_CODE_ADDR         0x16000

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

#define BASE_PD_KERNEL          0x27000
#define BASE_PT_KERNEL(pag)     (0x28000 + (pag << 12))


// Variable global que registra la proxima pagina disponible en el area libre
extern uint prox_pag_libre;

// Variables globales con las direcciones fisicas de las paginas compartidas de cada jugador
extern uint shared_jugador_a;
extern uint shared_jugador_b;

void mmu_inicializar();

// devuelve la proxima pagina libre del area libre del kernel
uint mmu_proxima_pagina_fisica_libre();

// setea en cero todos los bytes
void mmu_inicializar_pagina(uint * pagina);

// copia los bytes
void mmu_copiar_pagina    (uint src, uint dst);

// pide una pagina para usar de directorio. Luego inicializa las entradas que iran con identity mapping.
uint mmu_inicializar_dir_kernel();

// transforma coordenadas (x,y) en direcciones fisicas
uint mmu_xy2fisica(uint x, uint y);

// transforma coordenadas (x,y) en direcciones virtuales
uint mmu_xy2virtual(uint x, uint y);

// crea el directorio, las paginas, copia el codigo e inicializa el stack
uint mmu_inicializar_memoria_perro(perro_t *perro, int index_jugador, int index_tipo);

// debe remapear y copiar el codigo
void mmu_mover_perro(perro_t *perro, int viejo_x, int viejo_y);


void mmu_mapear_pagina  (uint virtual, uint cr3, uint fisica, uint attrs);
uint mmu_unmapear_pagina(uint virtual, uint cr3);

void mmu_perro_prueba();

#endif	/* !__MMU_H__ */
