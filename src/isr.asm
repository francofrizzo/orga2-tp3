; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

TSS_IDLE equ (21 << 3)

BITS 32

;; Pantalla
extern print
extern print_dec

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_atender_tick
extern sched_tarea_actual

;; Rutinas de alto nivel
extern game_atender_tick
extern game_atender_teclado
extern game_syscall_manejar
extern game_perro_termino

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    ; imprimimos un mensaje de error
    mov ebx, %1

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
    add esp, 2 + 4 * 4
    
    ; eliminamos la tarea que causo la excepcion

    call sched_tarea_actual

    push eax
    call game_perro_termino
    add esp, 4

    jmp TSS_IDLE:0

    jmp $ ; por si algo sale mal

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;

excepcion_msg db  "Se produjo una excepcion de tipo", 0
excepcion_len equ $ - excepcion_msg

; Scheduler
sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

; Syscalls
syscall_ret_temp:       dd 0x00

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
    
    call sched_atender_tick

    str cx
    cmp ax, cx
    je .fin

    ; cmp ax, TSS_IDLE
    ; je .no_break
    ; xchg bx, bx
    ; .no_break:

    mov [sched_tarea_selector], ax
    jmp far [sched_tarea_offset]
    
    .fin:
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

global _isr70
_isr70:
    
    ; Pusheamos los registros
    pushad
    
    ; Apilamos los parametros y resolvemos la syscall
    push ecx
    push eax
    call game_syscall_manejar
    add esp, 2 * 4
    
    ; Chequeamos que no estemos en la tarea idle
    str cx
    cmp cx, TSS_IDLE
    je .fin  ; Si la tarea era la idle, simplemente retornamos
    
    ; Pasamos a la tarea idle hasta que termine el quantum
    jmp TSS_IDLE:0

    .fin:
        mov [syscall_ret_temp], eax  ; salvamos el valor de retorno en una variable temporal
        popad                        ; recuperamos los registros
        mov eax, [syscall_ret_temp]  ; en eax volvemos a poner el valor de retorno
        iret                         ; fin de la rutina
