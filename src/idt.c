
/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0, 0);
        ...
        IDT_ENTRY(19, 0);

        ...
    }
*/

/*
// descomentar para usar, seteando segsel y attr segun corresponda

#define IDT_ENTRY(numero, dpl)                                                                                   \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x00;                                                                  \
    idt[numero].attr = (unsigned short) 0x0000 | (((unsigned short)(dpl & 0x3)) << 13);                          \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);
*/


void idt_inicializar() {
    // Excepciones

	// llamar a IDT_ENTRY una vez por cada entrada a configurar (es una macro que no soporta loops)
}
