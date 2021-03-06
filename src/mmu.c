/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "screen.h"

/* Implementacion de funciones del manejador de memoria */
/* -------------------------------------------------------------------------- */

void mmu_mapear_pagina (uint virtual, uint cr3, uint fisica, uint attrs) {
    uint pd_index = virtual >> 22;                   // nos quedamos con los 10 bits más significativos
    uint pt_index = (virtual >> 12) & 0x000003FF;    // nos quedamos con los 10 que siguen
    
    uint* pd = (uint*) (cr3 & 0xFFFFF000);           // usamos la info de CR3 para encontrar el directorio
    uint* pt;

    if ((pd[pd_index] & 1) == 0) {                   // si la tabla no esta presente, la creamos
        uint nueva_pag;
        if ((uint) pd == BASE_PD_KERNEL) {
            nueva_pag = BASE_PT_KERNEL(pd_index);    // nueva tabla para kernel
        } else {
            nueva_pag = mmu_proxima_pagina_fisica_libre();  // nueva tabla para usuario
        }
        pt = (uint*) nueva_pag;
        mmu_inicializar_pagina(pt);
        pd[pd_index] = nueva_pag | PAGE_ATTR_USER | PAGE_ATTR_READ_WRITE | PAGE_ATTR_PRESENT;
    } else {
        pt = (uint*) (pd[pd_index] & 0xFFFFF000);    // buscamos a la tabla de paginas en el directorio
    }

    pt[pt_index] = (fisica & 0xFFFFF000) | (attrs & 0x00000FFF);
    tlbflush();
}

uint mmu_inicializar_dir_kernel() {
    uint* pd_kernel = (uint*) (BASE_PD_KERNEL & 0xFFFFF000);

    mmu_inicializar_pagina(pd_kernel);                // limpiamos el directorio

    pd_kernel[0] = (BASE_PT_KERNEL(0) & 0xFFFFF000) | PAGE_ATTR_READ_WRITE | PAGE_ATTR_PRESENT;  // seteamos la primera entrada del directorio

    int i;
    for (i = 0; i < 1024; i++) {                      // mapeamos con identity mapping las primeras 1024 paginas
        mmu_mapear_pagina(i << 12, (uint) pd_kernel, i << 12, PAGE_ATTR_READ_WRITE | PAGE_ATTR_PRESENT);
    }

    return (uint) pd_kernel;
}

uint mmu_unmapear_pagina (uint virtual, uint cr3) {
    uint pd_index = virtual >> 22;                        // nos quedamos con los 10 bits más significativos
    uint pt_index = (virtual >> 12) & 0x000003FF;         // nos quedamos con los 10 que siguen
    
    uint* pd = (uint*) (cr3 & 0xFFFFF000);                // usamos la info de CR3 para encontrar el directorio

    if ((pd[pd_index] & 1) != 0) {                        // chequeamos que la tabla este presente
        uint* pt = (uint*) (pd[pd_index] & 0xFFFFF000);   // buscamos a la tabla de paginas en el directorio
        pt[pt_index] = pt[pt_index] & 0xFFFFFFFE;         // ponemos en 0 el bit de presente de la pagina
        tlbflush();                                       // limpiamos el tlb
        return 1;
    } else {
        return 0;
    }
}

uint prox_pag_libre = 0;
uint shared_jugador_a = 0;
uint shared_jugador_b = 0;

void mmu_inicializar() {
    prox_pag_libre = AREA_LIBRE_BASE;
    shared_jugador_a = mmu_proxima_pagina_fisica_libre();
    shared_jugador_b = mmu_proxima_pagina_fisica_libre();
}

uint mmu_proxima_pagina_fisica_libre() {
    uint res = prox_pag_libre;
    if (prox_pag_libre < AREA_LIBRE_LIM) {
        prox_pag_libre = prox_pag_libre + PAGE_SIZE;  // ATENCION: si se acaba el area libre se rompe todo
    }
    return res;
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
    uint* pd = (uint*) mmu_proxima_pagina_fisica_libre();
    mmu_inicializar_pagina(pd);

    // Identity mapping
    int i;
    for (i = 0; i < 1024; i++) {
        mmu_mapear_pagina(i << 12, (uint) pd, i << 12, PAGE_ATTR_READ_WRITE | PAGE_ATTR_PRESENT);
    }

    // Memoria compartida con otros perros del jugador
    uint shared_page;
    switch (index_jugador) {
        case JUGADOR_A: shared_page = shared_jugador_a; break;
        case JUGADOR_B: shared_page = shared_jugador_b; break;
        default: break;
    }

    mmu_mapear_pagina(
        SHARED_BASE,
        (uint) pd,
        shared_page,
        PAGE_ATTR_USER | PAGE_ATTR_READ_WRITE | PAGE_ATTR_PRESENT
    );

    // Pagina para codigo y stack
    mmu_mapear_pagina(
        CODIGO_BASE,
        (uint) pd,
        mmu_xy2fisica(perro->x, perro->y),
        PAGE_ATTR_USER | PAGE_ATTR_READ_WRITE | PAGE_ATTR_PRESENT
    );

    // Primera pagina del mapa
    mmu_mapear_pagina(
        mmu_xy2virtual(perro->x, perro->y),
        (uint) pd,
        mmu_xy2fisica(perro->x, perro->y),
        PAGE_ATTR_USER | PAGE_ATTR_PRESENT
    );

    // Copiamos el codigo del perro a su ubicacion en el mapa
    mmu_mapear_pagina(                    // Mapeamos temporalmente la direccion
        0x402000,                         // de la cucha en la direccion 0x402000
        rcr3(),                           // para poder copiar el codigo del perro
        mmu_xy2fisica(perro->x, perro->y),
        PAGE_ATTR_READ_WRITE | PAGE_ATTR_PRESENT
    );

    mmu_copiar_pagina(TASK_PERRO_CODE_ADDR(index_jugador, index_tipo), 0x402000);   // Copiamos el codigo del perro a la cucha

    // Ponemos los argumentos en el stack de la tarea
    uint* stack = (uint*) (0x403000 - 12);
    stack[0] = perro->jugador->x_cucha;
    stack[1] = perro->jugador->y_cucha;

    mmu_unmapear_pagina(0x402000, rcr3());    // Desmapeamos la cucha

    return (uint) pd;
}

void mmu_mover_perro(perro_t *perro, int viejo_x, int viejo_y) {
    // Copiamos el codigo del perro a su nueva ubicacion en el mapa
    mmu_mapear_pagina(         // Mapeamos temporalmente la nueva ubicacion
        0x402000,              // del perro en la direccion 0x402000 con permisos
        rcr3(),                // de kernel para poder copiar el codigo
        mmu_xy2fisica(perro->x, perro->y),
        PAGE_ATTR_READ_WRITE | PAGE_ATTR_PRESENT
    );


    mmu_copiar_pagina(0x401000, 0x402000);   // Copiamos el codigo del perro a la nueva ubicacion

    mmu_unmapear_pagina(0x402000, rcr3());    // Desmapeamos la nueva ubicacion

    // Mapeamos la nueva ubicacion del perro para codigo y stack
    mmu_mapear_pagina(
        CODIGO_BASE,
        (uint) rcr3(),
        mmu_xy2fisica(perro->x, perro->y),
        PAGE_ATTR_USER | PAGE_ATTR_READ_WRITE | PAGE_ATTR_PRESENT
    );

    // Mapeamos la nueva ubicacion read only y con permisos de usuario
    mmu_mapear_pagina(
        mmu_xy2virtual(perro->x, perro->y),
        (uint) rcr3(),
        mmu_xy2fisica(perro->x, perro->y),
        PAGE_ATTR_USER | PAGE_ATTR_PRESENT
    );
}

uint mmu_xy2fisica(uint x, uint y) {
    return MAPA_BASE_FISICA + (y * PAGE_SIZE * MAPA_ANCHO) + (x * PAGE_SIZE);
}

uint mmu_xy2virtual(uint x, uint y) {
    return MAPA_BASE_VIRTUAL + (y * PAGE_SIZE * MAPA_ANCHO) + (x * PAGE_SIZE);
}

void mmu_perro_prueba() {
    perro_t winnie;
    winnie.x = 25;
    winnie.y = 15;

    uint pd_winnie = mmu_inicializar_memoria_perro(&winnie, 1, 0);
    uint old_cr3 = rcr3();

    lcr3(pd_winnie);

    screen_pintar(' ', C_BG_MAGENTA, 0, 0);

    lcr3(old_cr3);
}
