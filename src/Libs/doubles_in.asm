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
        
        xor rax, rax            ; syscall 0 (read)
        xor rdi, rdi            ; stdin
        mov rsi, 0x400820       ; buf VA
        mov rdx, 0x20           ; buf length

        syscall

        mov rsi, 0x400820       ; buf VA
        xor rax, rax            ; rax is result
        mov r8, 10              ; multiplier
        xor rcx, rcx            ; I'll store chars here

        cmp byte [rsi], '-'
        jne .Convertation

        inc rsi

.Convertation:
        mul r8                  

        mov cl, byte [rsi]

        add rax, rcx   

        sub rax, '0'            

        inc rsi                 ; rax = 10 * rax + (*(rsi++) - '0')

        cmp [rsi], byte 0xa     ; while not '\n'
        ja .Convertation

        cmp byte [0x400820], '-'
        jne .NotNeg

        neg rax

.NotNeg:

        cvtsi2sd xmm0, rax

        pop rbp
        ret
