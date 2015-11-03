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

#define BASE_PD_KERNEL          0x27000
#define BASE_PT_KERNEL(pag)     (0x28000 + (pag << 12))


/* Implementacion de funciones del manejador de memoria */
/* -------------------------------------------------------------------------- */

void mmu_mapear_pagina (uint virtual, uint cr3, uint fisica, uint attrs) {
    uint pd_index = virtual >> 22;                   // nos quedamos con los 10 bits más significativos
    uint pt_index = (virtual >> 12) & 0x000003FF;    // nos quedamos con los 10 que siguen
    
    uint* pd = (uint*) (cr3 & 0xFFFFF000);           // usamos la info de CR3 para encontrar el directorio
    uint* pt = (uint*) (pd[pd_index] & 0xFFFFF000);  // buscamos a la tabla de paginas en el directorio

    pt[pt_index] = (fisica & 0xFFFFF000) | (attrs & 0x00000FFF);
}

uint mmu_inicializar_dir_kernel() {
    uint* pd_kernel = (uint*) (BASE_PD_KERNEL & 0xFFFFF000);

    mmu_inicializar_pagina(pd_kernel);                // limpiamos el directorio

    pd_kernel[0] = (BASE_PT_KERNEL(0) & 0xFFFFF000) | 0x3;  // seteamos la primera entrada del directorio, atributos: r/w, p

    int i;
    for (i = 0; i < 1024; i++) {                      // mapeamos con identity mapping las primeras 1024 paginas
        mmu_mapear_pagina(i << 12, (uint) pd_kernel, i << 12, 0x3);
    }

    return (uint) pd_kernel;
}

uint mmu_unmapear_pagina (uint virtual, uint cr3) {
    uint pd_index = virtual >> 22;                    // nos quedamos con los 10 bits más significativos
    uint pt_index = (virtual >> 12) & 0x000003FF;     // nos quedamos con los 10 que siguen
    
    uint* pd = (uint*) (cr3 & 0xFFFFF000);            // usamos la info de CR3 para encontrar el directorio
    uint* pt = (uint*) (pd[pd_index] & 0xFFFFF000);   // buscamos a la tabla de paginas en el directorio

    pt[pt_index] = pt[pt_index] & 0xFFFFFFFE;         // ponemos en 0 el bit de presente

    return 0;
}

uint prox_pag_libre = 0;

void mmu_inicializar() {
    prox_pag_libre = AREA_LIBRE_BASE;
}

uint mmu_proxima_pagina_fisica_libre() {
    return prox_pag_libre;
}

void mmu_inicializar_pagina(uint * pagina) {
    int i;
    for (i = 0; i < 1024; i++) {
        pagina[i] = 0;
    }
}

void mmu_copiar_pagina(uint src, uint dst) {
    uint* src_ptr = (uint*) src;
    uint* dst_ptr = (uint*) dst;

    int i;
    for (i = 0; i < 1024; i++) {
        dst_ptr[i] = src_ptr[i];
    }
}

uint mmu_inicializar_memoria_perro(perro_t *perro, int index_jugador, int index_tipo) {
    
}
