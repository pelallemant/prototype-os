%define BASE    0x100  ; 0x0100:0x0 = 0x1000  -> the offset of the kernel _start routine
%define KSIZE   50     ; nombre de secteurs a charger

[BITS 16]
[ORG 0x0]

jmp start
start:

; The CPU (i386) is currently in real mode
;
; initialize the segments into the adress 0x07C0:0X0 = 0x7C00
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax
    mov ax, 0x8000 ; stack in 0xFFFF
    mov ss, ax
    mov sp, 0xf000

; we get the boot drive into bootdrv
    mov [bootdrv], dl  ; dl is written into bootdrv

; loading the kernel into the RAM

; first call to a BIOS INTERUPT: init
    xor ax, ax   ; ax = 0x0000 => now ah = 0x00
    int 0x13     ; BIOS INTERUPT: int 0x13 with ah=0x0: Reset Disk Drive

; second call to a BIOS INTERUPT: copy the kernel
    push es           ; save "extra segment" register state
    mov ax, BASE      
    mov es, ax        ; "mov es, BASE" doesn't exists
    mov bx, 0         ; es:bx -> buffer adress pointer BASE:0x0
    mov ah, 2         ; 0x02
    mov al, KSIZE     ; sectors to read count
    mov ch, 0         ; cylinder
    mov cl, 2         ; sector
    mov dh, 0         ; head
    mov dl, [bootdrv] ; dl = drive
    int 0x13          ; BIOS INTERUPT: int 0x13 with ah = 0x02: Read Sectors From Drive
    pop es            ; load es state

; initialization of the gdt pointer
    mov ax, gdtend    ; calculate the GDT limit
    mov bx, gdt
    sub ax, bx
    mov word [gdtptr], ax

    xor eax, eax      ; calcule l'adresse lineaire de GDT
    xor ebx, ebx
    mov ax, ds
    mov ecx, eax
    shl ecx, 4
    mov bx, gdt
    add ecx, ebx
    mov dword [gdtptr+2], ecx

; we leave real mode to protected mode
    cli ; disable interrupts
    lgdt [gdtptr]    ; load the gdt
    mov eax, cr0
    or  ax, 1
    mov cr0, eax        ; PE set to 1 (CR0)

; clear CPU cache
; do not remove the 2 following lines
    jmp next  
next:

; reinitialization of data segment

    mov ax, 0x10
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov es, ax

; reinitialization of stack segment

    mov ss, ax
    mov esp, 0x9F000

; now we call the kernel

    jmp dword 0x8:0x1000 ; jump to kernel routine

;--------------------------------------------------------------------
bootdrv:  db 0
;--------------------------------------------------------------------
gdt:
    db 0, 0, 0, 0, 0, 0, 0, 0
gdt_cs:
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10011011b, 11011111b, 0x0
gdt_ds:
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10010011b, 11011111b, 0x0
gdtend:
;--------------------------------------------------------------------
gdtptr:
    dw 0  ; limite
    dd 0  ; base
;--------------------------------------------------------------------

;; NOP jusqu'a 510
times 510-($-$$) db 144
dw 0xAA55
