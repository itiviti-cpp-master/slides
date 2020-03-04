        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     QWORD PTR [rbp-8], rdi
        mov     eax, esi
        mov     BYTE PTR [rbp-12], al
        movsx   eax, BYTE PTR [rbp-12]
        cmp     eax, 67
        je      .L2
        cmp     eax, 67
        jg      .L3
        cmp     eax, 65
        je      .L4
        jmp     .L5
.L3:
        cmp     eax, 71
        je      .L6
        cmp     eax, 84
        je      .L7
        jmp     .L5
.L4:
        mov     eax, 84
        jmp     .L8
