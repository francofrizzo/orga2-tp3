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
extern screen_show_debug_info

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
;; Estructura y variables de modo debug
;; -------------------------------------------------------------------------- ;;
extern debug_mode
extern debug_data

struc ddt
    .tipo       resw 1
    .mnemonico  resb 4
    .eax        resd 1
    .ebx        resd 1
    .ecx        resd 1
    .edx        resd 1
    .esi        resd 1
    .edi        resd 1
    .ebp        resd 1
    .esp        resd 1
    .eip        resd 1
    .cs         resw 1
    .ds         resw 1
    .es         resw 1
    .fs         resw 1
    .gs         resw 1
    .ss         resw 1
    .eflags     resd 1
    .cr0        resd 1
    .cr2        resd 1
    .cr3        resd 1
    .cr4        resd 1
    .error_code resd 1
    .stack      resd 5
endstruc

;;
;; Mnemonicos de excepciones
;; -------------------------------------------------------------------------- ;;

%define mnemonico_0  `#DE\0`
%define mnemonico_1  `#DB\0`
%define mnemonico_2  `\0\0\0\0`
%define mnemonico_3  `#BP\0`
%define mnemonico_4  `#OF\0`
%define mnemonico_5  `#BR\0`
%define mnemonico_6  `#UD\0`
%define mnemonico_7  `#NM\0`
%define mnemonico_8  `#DF\0`
%define mnemonico_9  `\0\0\0\0`
%define mnemonico_10 `#TS\0`
%define mnemonico_11 `#NP\0`
%define mnemonico_12 `#SS\0`
%define mnemonico_13 `#GP\0`
%define mnemonico_14 `#PF\0`
%define mnemonico_15 `\0\0\0\0`
%define mnemonico_16 `#MF\0`
%define mnemonico_17 `#AC\0`
%define mnemonico_18 `#MC\0`
%define mnemonico_19 `#XM\0`

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    ; Salvamos eax
    mov       [debug_data + ddt.eax], eax

    ; Vemos si el modo debug esta activo
    mov       eax, [debug_mode]
    cmp       eax, 0
    je        .chau_tarea

        ; Guardamos todos los registros para mostrarlos en pantalla
        
        ; Codigo de error y mnemonico
        mov word   [debug_data + ddt.tipo], %1
        mov dword  [debug_data + ddt.mnemonico], mnemonico_%1

        ; Registros de proposito general
        mov       [debug_data + ddt.ebx], ebx
        mov       [debug_data + ddt.ecx], ecx
        mov       [debug_data + ddt.edx], edx
        mov       [debug_data + ddt.esi], esi
        mov       [debug_data + ddt.edi], edi
        mov       [debug_data + ddt.ebp], ebp
        mov       [debug_data + ddt.ds ], ds
        mov       [debug_data + ddt.es ], es
        mov       [debug_data + ddt.fs ], fs
        mov       [debug_data + ddt.gs ], gs

        ; Registros de control
        mov       eax, cr0
        mov       [debug_data + ddt.cr0], eax
        mov       eax, cr2
        mov       [debug_data + ddt.cr2], eax
        mov       eax, cr3
        mov       [debug_data + ddt.cr3], eax
        mov       eax, cr4
        mov       [debug_data + ddt.cr4], eax

        ; Registros en stack
        mov       eax, [esp]
        mov       [debug_data + ddt.error_code], eax
        mov       eax, [esp + 4]
        mov       [debug_data + ddt.eip], eax
        mov       ax, [esp + 8]
        mov       [debug_data + ddt.cs], ax
        mov       eax, [esp + 12]
        mov       [debug_data + ddt.eflags], eax
        mov       ecx, [esp + 16]
        mov       [debug_data + ddt.esp], ecx
        mov       ax, [esp + 20]
        mov       [debug_data + ddt.ss], ax

        ; Stack
        mov       eax, [ecx]
        mov       [debug_data + ddt.stack], eax
        mov       eax, [ecx + 4]
        mov       [debug_data + ddt.stack + 4], eax
        mov       eax, [ecx + 8]
        mov       [debug_data + ddt.stack + 8], eax
        mov       eax, [ecx + 12]
        mov       [debug_data + ddt.stack + 12], eax
        mov       eax, [ecx + 16]
        mov       [debug_data + ddt.stack + 16], eax
    
    ; Eliminamos la tarea que causo la excepcion
    .chau_tarea:
        call sched_tarea_actual

        push eax
        call game_perro_termino
        add esp, 4

    ; Vemos (de nuevo) si el modo debug esta activo
    mov       eax, [debug_mode]
    cmp       eax, 0
    je        .fin
    
        ; Mostramos en pantalla la info de debug
        call screen_show_debug_info

    ; Saltamos a la tarea idle
    .fin:
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
