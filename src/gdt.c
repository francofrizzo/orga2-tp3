/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#include "tss.h"




/* Definicion de la GDT */
/* -------------------------------------------------------------------------- */

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    [8] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x01,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    [9] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x01,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    [10] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x01,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    [11] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x01,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    [12] = (gdt_entry) {
        (unsigned short)    0x1F3F,         /* limit[0:15]  */
        (unsigned short)    0x8000,         /* base[0:15]   */
        (unsigned char)     0x0B,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    [20] = (gdt_entry) {
        (unsigned short)    TSS_SIZE & 0x0000FFFF,                   /* limit[0:15]  */
        (unsigned short)    &tss_inicial & 0x0000FFFF,               /* base[0:15]   */
        (unsigned char)     (&tss_inicial >> 16) & 0x000000FF,       /* base[23:16]  */
        (unsigned char)     0x09,                                    /* type         */
        (unsigned char)     0x00,                                    /* s            */
        (unsigned char)     0x00,                                    /* dpl          */
        (unsigned char)     0x01,                                    /* p            */
        (unsigned char)     (TSS_SIZE >> 16) & 0x0000000F,           /* limit[16:19] */
        (unsigned char)     0x01,                                    /* avl          */
        (unsigned char)     0x00,                                    /* l            */
        (unsigned char)     0x00,                                    /* db           */
        (unsigned char)     0x00,                                    /* g            */
        (unsigned char)     (TSS_SIZE >> 24) & 0x00000000FF,         /* base[31:24]  */
    },

    [21] = (gdt_entry) {
        (unsigned short)    TSS_SIZE & 0x0000FFFF,                /* limit[0:15]  */
        (unsigned short)    &tss_idle & 0x0000FFFF,               /* base[0:15]   */
        (unsigned char)     (&tss_idle >> 16) & 0x000000FF,       /* base[23:16]  */
        (unsigned char)     0x09,                                 /* type         */
        (unsigned char)     0x00,                                 /* s            */
        (unsigned char)     0x00,                                 /* dpl          */
        (unsigned char)     0x01,                                 /* p            */
        (unsigned char)     (TSS_SIZE >> 16) & 0x0000000F,        /* limit[16:19] */
        (unsigned char)     0x01,                                 /* avl          */
        (unsigned char)     0x00,                                 /* l            */
        (unsigned char)     0x00,                                 /* db           */
        (unsigned char)     0x00,                                 /* g            */
        (unsigned char)     (TSS_SIZE >> 24) & 0x00000000FF,      /* base[31:24]  */
    },

};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};

