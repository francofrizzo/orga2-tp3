; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; Pantalla
extern screen_limpiar
extern print
extern print_dec

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_atender_tick
extern sched_tarea_actual

;; Rutinas de alto nivel
extern game_atender_tick
extern game_perro_actual
extern game_atender_teclado
extern game_syscall_manejar

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    mov ebx, %1

    call screen_limpiar

    push word  0x07           ; attr
    push dword 0              ; y
    push dword 0              ; x
    push excepcion_msg        ; text
    call print
    add esp, 2 + 3 * 4

    push word  0x07           ; attr
    push dword 0              ; y
    push dword excepcion_len  ; x
    push dword 2              ; size
    push ebx                  ; numero
    call print_dec
    
    jmp $

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;

excepcion_msg db  "Se produjo una excepcion de tipo", 0
excepcion_len equ $ - excepcion_msg

; Scheduler

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

global _isr32
_isr32:
    
    pushad
    call fin_intr_pic1
    
    push game_perro_actual
    call game_atender_tick
    add esp, 4
    
    popad
    iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33
_isr33:
    
    pushad
    call fin_intr_pic1
    
    in al, 0x60
    push ax
    call game_atender_teclado
    add esp, 2
    
    popad
    iret

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

global _isr46
_isr46:
    
    pushad
    
    push ecx
    push eax
    call game_syscall_manejar
    add esp, 2 * 4
    
    popad
    mov eax, 0x42

    iret

