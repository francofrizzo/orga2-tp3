/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Tipos basicos */
/* -------------------------------------------------------------------------- */
#define NULL                    0
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

/* Constantes basicas */
/* -------------------------------------------------------------------------- */
#define PAGE_SIZE               0x00001000
#define TASK_SIZE               4096

#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */

#define BASE_PILA_KERNEL        0x00027000

#define GDT_DPL_KERNEL          0
#define GDT_DPL_USER            3

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT               48

#define GDT_IDX_NULL_DESC       0
#define GDT_IDX_CODE_KERNEL     8
#define GDT_IDX_CODE_USER       9
#define GDT_IDX_DATA_KERNEL     10
#define GDT_IDX_DATA_USER       11
#define GDT_IDX_VIDEO           12
#define GDT_IDX_TSS_INICIAL     20
#define GDT_IDX_TSS_IDLE        21

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC       (GDT_IDX_NULL_DESC      << 3)
#define GDT_OFF_CODE_KERNEL     (GDT_IDX_CODE_KERNEL    << 3)
#define GDT_OFF_CODE_USER       (GDT_IDX_CODE_USER      << 3)
#define GDT_OFF_DATA_KERNEL     (GDT_IDX_DATA_KERNEL    << 3)
#define GDT_OFF_DATA_USER       (GDT_IDX_DATA_USER      << 3)
#define GDT_OFF_VIDEO           (GDT_IDX_VIDEO          << 3)
#define GDT_OFF_TSS_INICIAL     (GDT_IDX_TSS_INICIAL    << 3)
#define GDT_OFF_TSS_IDLE        (GDT_IDX_TSS_IDLE       << 3)

/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */
#define SEG_CODE_KERNEL         (GDT_OFF_CODE_KERNEL | GDT_DPL_KERNEL)
#define SEG_CODE_USER           (GDT_OFF_CODE_USER   |   GDT_DPL_USER)
#define SEG_DATA_KERNEL         (GDT_OFF_DATA_KERNEL | GDT_DPL_KERNEL)
#define SEG_DATA_USER           (GDT_OFF_DATA_USER   |   GDT_DPL_USER)
#define SEG_VIDEO               (GDT_OFF_VIDEO       | GDT_DPL_KERNEL)
#define TSS_INICIAL             (GDT_OFF_TSS_INICIAL | GDT_DPL_KERNEL)
#define TSS_IDLE                (GDT_OFF_TSS_IDLE    | GDT_DPL_KERNEL)

#endif  /* !__DEFINES_H__ */
