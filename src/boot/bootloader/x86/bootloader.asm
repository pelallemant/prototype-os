%define BASE    0x100  ; 0x0100:0x0 = 0x1000  -> the offset of the kernel _start routine
%define KSIZE   50     ; nombre de secteurs a charger

[BITS 16]
[ORG 0x0]

jmp start

;--------------------------------------------------------------------
; CalcAdr
; Entree:
;    ax -> segment
;    bx -> offset
; Sortie:
;    ecx -> adresse lineaire sur 32 bits
; Modifie: partie haute de ebx
;--------------------------------------------------------------------
calcadr:
    xor ecx, ecx
    mov cx, ax
    shl ecx, 4
    and ebx, 0x0000FFFF
    add ecx, ebx
    ret
;--------------------------------------------------------------------

;--------------------------------------------------------------------
; descInit
;--------------------------------------------------------------------
%macro descInit    5    ; base(32), limite(20/32), acces(8), flags(4/8), adresse(32)
    push eax
; base :
    mov eax, %1
    mov word [%5+2], ax
    shr eax, 16
    mov byte [%5+4], al
    shr eax, 8
    mov byte [%5+7], al
; limite :
    mov eax, %2
    and eax, 0x000FFFFF
    mov word [%5], ax  ; ecrit (0..15)
    shr eax, 16        ; place (16..19) sur le nibble inferieur
    mov byte [%5+6], 0 ; initialise flags+lim(16..19) a 0
    or  [%5+6], al     ; ecrit (16..19)
; flags :
    mov al, %4
    and al, 0x0F
    shl al, 4
    or [%5+6], al
; acces :
    mov byte [%5+5], %3
    pop eax
%endmacro
;--------------------------------------------------------------------


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

; initialisation de la GDT
    ; descInit base(32), limite(20/32), acces/type(8), flags(4/8), adresse(16)
    descInit 0, 0xFFFFF, 10011011b, 1101b, gdt_cs
    descInit 0, 0xFFFFF, 10010011b, 1101b, gdt_ds

; initialization of the gdt pointer
    mov ax, gdtend    ; calculate the GDT limit
    mov bx, gdt
    sub ax, bx
    mov word [gdtptr], ax

;    xor eax, eax      ; calcule l'adresse lineaire de GDT
;    xor ebx, ebx
;    mov ax, ds
;    mov ecx, eax
;    shl ecx, 4
;    mov bx, gdt
;    add ecx, ebx
;    mov dword [gdtptr+2], ecx

    xor eax, eax    ; calcule l'adresse lineaire de GDT
    mov  ax, ds
    mov  bx, gdt
    call calcadr
    mov dword [gdtptr+2], ecx

; we leave real mode to protected mode

    cli ; disable interrupts
    lgdt [gdtptr]    ; load the gdt

; CR0 first bit set to 1 -> enable protected mode 
    mov eax, cr0
    or  ax, 1
    mov cr0, eax
; now we are in protected mode

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
;gdt:
;    db 0, 0, 0, 0, 0, 0, 0, 0
;gdt_cs:
;    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10011011b, 11011111b, 0x0
;gdt_ds:
;    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10010011b, 11011111b, 0x0
;gdtend:

gdt:
gdt_null:
    dw 0, 0, 0, 0
gdt_cs:
    dw 0, 0, 0, 0
gdt_ds:
    dw 0, 0, 0, 0
gdtend:

;--------------------------------------------------------------------
gdtptr:
    dw 0  ; limite
    dd 0  ; base
;--------------------------------------------------------------------

;; NOP jusqu'a 510
times 510-($-$$) db 144
dw 0xAA55
