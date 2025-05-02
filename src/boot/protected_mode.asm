

;
; usa "jmp load_protected" per attivare in automatico la proitected mode
; La GDT ha impostato come segmenti di default quelli descritti da OSDev
;

load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:load32


; GDT
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:     ; CS SHOULD POINT TO THIS
    dw 0xffff ; Segment limit first 0-15 bits
    dw 0      ; Base first 0-15 bits
    db 0      ; Base 16-23 bits
    db 0x9a   ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

; offset 0x10
gdt_data:      ; DS, SS, ES, FS, GS
    dw 0xffff ; Segment limit first 0-15 bits
    dw 0      ; Base first 0-15 bits
    db 0      ; Base 16-23 bits
    db 0x92   ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

gdt_end:


gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start


[BITS 32]

%include "./src/boot/ata_lba_reader_32.asm"

load32:
    mov eax, 1         ; dico da dove partire a leggere dal disco
    mov ecx, 100       ; numero settori da leggere
    mov edi, 0x0100000 ; da dove partire a caricare in ram
    call ata_lba_read

    jmp CODE_SEG:0x0100000