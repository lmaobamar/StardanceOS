; src/boot.asm
; https://wiki.osdev.org/Multiboot was helpful
bits 32

section .multiboot_header
align 8
multiboot_start:
    dd 0xe85250d6                                            ; this is the magic number for multiboot 2
    dd 0                                                     ; architecture 0 = protected mode i386
    dd multiboot_end - multiboot_start                       ; header len
    dd -(0xe85250d6 + 0 + (multiboot_end - multiboot_start)) ; checksum

    ; end tag
    dw 0
    dw 0
    dd 8
multiboot_end:

section .text
global start
extern kernel_main

start:
    ; make a minimal stack so we can handoff to C (i think that's how it goes?)
    mov esp, sttop
    call kernel_main

.halt:
    hlt
    jmp .halt

section .bss
align 16
stbottom:
    resb 16384
sttop: