
section .asm

gdt_flush:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 12]
    lgdt [eax]

    leave
    ret
