; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

;;
;; Variables y funciones externas
;; -------------------------------------------------------------------------- ;;

;; Segmentacion
extern GDT_DESC

;; Interrupciones
extern IDT_DESC
extern idt_inicializar
extern resetear_pic
extern habilitar_pic

;; Manejo de memoria
extern CR3_KERNEL
extern mmu_inicializar_dir_kernel
extern mmu_unmapear_pagina
extern mmu_inicializar
extern mmu_perro_prueba

;; TSS
extern tss_inicializar
extern tss_perro_prueba

;; Pantalla
extern print
PRINT_ARGS_SIZE equ 2 + 3 * 4
extern screen_pintar_rect
SCREEN_PINTAR_RECT_ARGS_SIZE equ 2 + 4 * 4
extern screen_inicializar


;;
;; Defines
;; -------------------------------------------------------------------------- ;;
BASE_PILA_KERNEL equ    0x27000
SEG_CODE_KERNEL  equ    0x40
SEG_CODE_USER    equ    0x4B
SEG_DATA_KERNEL  equ    0x50
SEG_DATA_USER    equ    0x5B
SEG_VIDEO        equ    0x60
TSS_INICIAL      equ    (20 << 3)
TSS_IDLE         equ    (21 << 3)

%define BREAKPOINT xchg bx, bx

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...', 0
iniciando_mp_len equ    $ - iniciando_mp_msg

nombre_grupo     db     'Smelly Cat', 0

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
    jmp SEG_CODE_KERNEL:modoprotegido

BITS 32

    modoprotegido:

    ; Establecer selectores de segmentos
    mov ax, SEG_DATA_KERNEL
    mov ds, ax
    mov ss, ax

    ; Establecer la base de la pila
    mov esp, BASE_PILA_KERNEL
    mov ebp, BASE_PILA_KERNEL

    ; Prueba de segmentación: pintamos la esquina de la pantalla usando un
    ; segmento exclusivo para video
    mov ax, SEG_VIDEO
    mov es, ax
    mov word [es:0x0], 0x0F41

    ; Imprimir mensaje de bienvenida
    push word  0x07           ; attr
    push dword 1              ; y
    push dword 0              ; x
    push iniciando_mp_msg     ; text
    call print
    add esp, PRINT_ARGS_SIZE

    ; Pintamos la pantalla (ejercicio 1d)
    
    ; pintar de negro primer fila
    push dword 80      ; ancho
    push dword 1       ; alto
    push dword 0       ; columna
    push dword 0       ; fila
    push 00000000b     ; color
    push ' '           ; letra
    call screen_pintar_rect
    add esp, SCREEN_PINTAR_RECT_ARGS_SIZE

    ; pintar de gris:
    push dword 80      ; ancho
    push dword 44      ; alto
    push dword 0       ; columna
    push dword 1       ; fila
    push 01110111b     ; color
    push ' '           ; letra
    call screen_pintar_rect
    add esp, SCREEN_PINTAR_RECT_ARGS_SIZE

    ; pintar ultimas filas de negro
    push dword 80      ; ancho
    push dword 5       ; alto
    push dword 0       ; columna
    push dword 45      ; fila
    push 00000000b     ; color
    push ' '           ; letra
    call screen_pintar_rect
    add esp, SCREEN_PINTAR_RECT_ARGS_SIZE

    ; pintar de rojo y azul
    push dword 7       ; ancho
    push dword 5       ; alto
    push dword 33      ; columna
    push dword 45      ; fila
    push 01000100b     ; color (rojo)
    push ' '           ; letra
    call screen_pintar_rect
    add esp, SCREEN_PINTAR_RECT_ARGS_SIZE

    push dword 7       ; ancho
    push dword 5       ; alto
    push dword 40      ; columna
    push dword 45      ; fila
    push 00010001b     ; color (azul)
    push ' '           ; letra
    call screen_pintar_rect
    add esp, SCREEN_PINTAR_RECT_ARGS_SIZE

    ; Inicializar el juego

    ; Inicializar pantalla
    call screen_inicializar

    ; Inicializar el manejador de memoria
    call mmu_inicializar

    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Probamos que siga andando todo imprimiendo el nombre del grupo
    push word  0x07        ; attr
    push dword 0           ; y
    push dword 70          ; x
    push nombre_grupo      ; text
    call print
    add esp, PRINT_ARGS_SIZE

    ; Desmapeamos la ultima pagina del kernel
    ; mov eax, cr3
    ; push eax               ; cr3
    ; push dword 0x3FF000    ; virtual
    ; call mmu_unmapear_pagina
    ; add esp, 2 * 4

    ; Ejercicio 4 c

    call mmu_perro_prueba

    ; Inicializar tss
    ; Inicializar tss de la tarea Idle
    call tss_inicializar

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones
    call resetear_pic
    call habilitar_pic

    ; Cargar tarea inicial
    mov ax, TSS_INICIAL
    ltr ax

    ; Habilitar interrupciones
    sti

    ; Saltar a la primera tarea: Idle
    call tss_perro_prueba
    BREAKPOINT
    jmp (22 << 3):0
    jmp TSS_IDLE:0

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
