push   %r12
push   %rbp
mov    $0x602200,%edi
push   %rbx
sub    $0x50,%rsp
mov    %fs:0x28,%rax

mov    %rax,0x48(%rsp)
xor    %eax,%eax
lea    0x10(%rsp),%rax
mov    %rsp,%rsi
movq   $0x0,0x8(%rsp)

movb   $0x0,0x10(%rsp)
movq   $0x0,0x28(%rsp)

mov    %rax,(%rsp)
lea    0x30(%rsp),%rax
movb   $0x0,0x30(%rsp)
mov    %rax,0x20(%rsp)
callq  400bc0
mov    0x8(%rsp),%rbx
mov    0x28(%rsp),%rbp
cmp    %rbp,%rbx
ja     400d82 <main+0x162>
jae    400c9c <main+0x7c>
mov    0x20(%rsp),%rax

