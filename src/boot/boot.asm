ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start


_start:
    jmp short start
    nop


times 33 db 0


start:
    jmp 0:step2

step2:
    cli ; Clear Interrupts
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti ; Enables Interrupts

    jmp load_protected


%include 'src/boot/protected_mode.asm'


times 510 - ($ - $$) db 0
dw 0xAA55