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
        lea rsi, [rel buf]
        mov rdx, 0x20

        syscall

        lea rsi, [rel buf]
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

buf db 0x20 dup(0)
