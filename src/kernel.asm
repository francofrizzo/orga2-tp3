; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

extern GDT_DESC
extern screen_pintar_rect

extern IDT_DESC
extern idt_inicializar

extern CR3_KERNEL
extern mmu_inicializar_dir_kernel

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

SEG_COD_KERNEL  equ 0x40
SEG_COD_USER    equ 0x4B
SEG_DATA_KERNEL equ 0x50
SEG_DATA_USER   equ 0x5B
SEG_VIDEO       equ 0x60

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

    ;pintar de negro primer fila
    ;void screen_pintar_rect(uchar letra, uchar color, int fila, int columna, int alto, int ancho)
    push dword 80
    push dword 1
    push dword 0
    push dword 0
    push 00000000b
    push ' '
    call screen_pintar_rect
    ;pintar de gris:
    ;void screen_pintar_rect(uchar letra, uchar color, int fila, int columna, int alto, int ancho)
    push dword 80
    push dword 44
    push dword 0
    push dword 1
    push 01110111b
    push ' '
    call screen_pintar_rect

    ;pintar ultimas filas de negro
    ;void screen_pintar_rect(uchar letra, uchar color, int fila, int columna, int alto, int ancho)
    push dword 80
    push dword 5
    push dword 0
    push dword 45
    push 00000000b
    push ' '
    call screen_pintar_rect

    ;pintar de rojo y azul
    ;void screen_pintar_rect(uchar letra, uchar color, int fila, int columna, int alto, int ancho)
    push dword 7
    push dword 5
    push dword 33
    push dword 45
    push 01000100b ;rojo
    push ' '
    call screen_pintar_rect

    ;void screen_pintar_rect(uchar letra, uchar color, int fila, int columna, int alto, int ancho)
    push dword 7
    push dword 5
    push dword 40
    push dword 45
    push 00010001b ;azul
    push ' '
    call screen_pintar_rect

    ; Inicializar el manejador de memoria

    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    mov eax, CR3_KERNEL
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

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
