; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

extern GDT_DESC

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    xchg bx, bx

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
    jmp 0x40:modoprotegido

BITS 32

    modoprotegido:

    ; Establecer selectores de segmentos
    mov cs, 0x40
    mov ds, 0x50
    mov ss, 0x50

    ; Establecer la base de la pila
    mov ebp, 0x27000
    mov esp, 0x27000

    mov es, 0x60
    mov word [0x60:0x0], 

    ; Imprimir mensaje de bienvenida

    ; Inicializar el juego

    ; Inicializar pantalla

    ; Inicializar el manejador de memoria

    ; Inicializar el directorio de paginas

    ; Cargar directorio de paginas

    ; Habilitar paginacion

    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT

    ; Cargar IDT

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
