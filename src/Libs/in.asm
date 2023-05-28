BITS 64

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; %d specifier
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; Entry:    32-bit int in stack
; Exit:     -
; Destroys: rax, rcx, rdx, rsi, rdi, r9
; Expects:  r8 for calculating stack arg offset
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

_in     push rbp
        
        xor rax, rax
        xor rdi, rdi
        mov rsi, 0x400820
        mov rdx, 0x20

        syscall

        mov rsi, 0x400820
        xor rax, rax
        mov r8, 10
        xor rcx, rcx

.Convertation:
        mul r8

        mov cl, byte [rsi]

        add rax, rcx

        sub rax, '0'

        inc rsi

        cmp [rsi], byte 10
        ja .Convertation

        pop rbp
        ret
