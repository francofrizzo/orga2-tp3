; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

;; Segmentacion
extern GDT_DESC

;; Interrupciones
extern IDT_DESC
extern idt_inicializar

;; Manejo de memoria
extern CR3_KERNEL
extern mmu_inicializar_dir_kernel
extern mmu_unmapear_pagina

;; Pantalla
extern print
extern screen_pintar_rect

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

nombre_grupo     db     'Smelly Cat', 0

SEG_COD_KERNEL   equ    0x40
SEG_COD_USER     equ    0x4B
SEG_DATA_KERNEL  equ    0x50
SEG_DATA_USER    equ    0x5B
SEG_VIDEO        equ    0x60

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0


    ; Habilitar A20
    call habilitar_A20

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 0x00000001
    mov cr0, eax

    ; Saltar a modo protegido
    jmp SEG_COD_KERNEL:modoprotegido

BITS 32

    modoprotegido:

    ; Establecer selectores de segmentos
    mov ax, SEG_DATA_KERNEL
    mov ds, ax
    mov ss, ax

    ; Establecer la base de la pila
    mov esp, 0x27000
    mov ebp, 0x27000

    mov ax, SEG_VIDEO
    mov es, ax


    mov word [es:0x0], 0x0F41



    ; Imprimir mensaje de bienvenida

    ; Inicializar el juego

    ; Inicializar pantalla

    ; pintar de negro primer fila
    push dword 80      ; ancho
    push dword 1       ; alto
    push dword 0       ; columna
    push dword 0       ; fila
    push 00000000b     ; color
    push ' '           ; letra
    call screen_pintar_rect
    add esp, 2 + 4 * 4

    ; pintar de gris:
    push dword 80      ; ancho
    push dword 44      ; alto
    push dword 0       ; columna
    push dword 1       ; fila
    push 01110111b     ; color
    push ' '           ; letra
    call screen_pintar_rect
    add esp, 2 + 4 * 4

    ; pintar ultimas filas de negro
    push dword 80      ; ancho
    push dword 5       ; alto
    push dword 0       ; columna
    push dword 45      ; fila
    push 00000000b     ; color
    push ' '           ; letra
    call screen_pintar_rect
    add esp, 2 + 4 * 4

    ; pintar de rojo y azul
    push dword 7       ; ancho
    push dword 5       ; alto
    push dword 33      ; columna
    push dword 45      ; fila
    push 01000100b     ; color (rojo)
    push ' '           ; letra
    call screen_pintar_rect
    add esp, 2 + 4 * 4

    push dword 7       ; ancho
    push dword 5       ; alto
    push dword 40      ; columna
    push dword 45      ; fila
    push 00010001b     ; color (azul)
    push ' '           ; letra
    call screen_pintar_rect
    add esp, 2 + 4 * 4

    ; Inicializar el manejador de memoria

    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Probamos que siga andando todo imprimiendo el nombre del grupo
    push word  0x0F        ; attr
    push dword 0           ; y
    push dword 70          ; x
    push nombre_grupo      ; text
    call print
    add esp, 2 + 3 * 4

    ; Desmapeamos la ultima pagina del kernel
    mov eax, cr3
    push eax               ; cr3
    push dword 0x3FF000    ; virtual
    call mmu_unmapear_pagina
    add esp, 2 * 4

    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    lidt [IDT_DESC]

    int 3

    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
