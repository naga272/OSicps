[BITS 32]

global _start
extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    ; Imposta i segmenti di dati
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Imposta lo stack
    mov ebp, 0x00200000
    mov esp, ebp

    ; Abilita la linea A20
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Rimappa il PIC master
    mov al, 00010001b
    out 0x20, al
    mov al, 0x20
    out 0x21, al
    mov al, 00000001b
    out 0x21, al

    ; enter to kernel
    call kernel_main
    jmp $


times 510-($-$$) db 0
