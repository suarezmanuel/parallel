# 340858935 Manuel Ignacio Suarez Del Solar
# 216270827 Azmon Avraham Zvi
.section .data

.align 16
# define unit vector in memory
_1_ps:
    .float 1.0, 1.0, 1.0, 1.0

.section .text
.globl formula2

formula2:
    pushq   %rbp
    movq    %rsp, %rbp

    # sum to zero 
    xorps   %xmm0, %xmm0           # xmm0 = 0.0, sum

    # vector of ones
    movaps  _1_ps(%rip), %xmm7  
    movaps  %xmm7, %xmm1         

    # loop variables
    movq    %rdx, %rcx             # rcx = n
    shrq    $2, %rcx               # rcx = n / 4 (number of SIMD iterations)
    xorq    %r8, %r8               # r8 = 0 (loop counter)

    # %rdi = *px
    # %rsi = *py

    # Start of the loop
.LoopStart:
    cmpq    %r8, %rcx              # compare loop counter with iteration count
    jle     .END_LOOP              # if r8 >= rcx, exit loop

    movups  (%rdi), %xmm2          # xmm2 = x = *px
    movups  (%rsi), %xmm3          # xmm3 = y = *py

    movaps  %xmm2, %xmm4           # xmm4 = x
    mulps   %xmm3, %xmm4           # xmm4 = x * y

    movaps  %xmm2, %xmm5           # xmm5 = x
    subps   %xmm3, %xmm5           # xmm5 = x - y

    movaps  %xmm5, %xmm6           # xmm6 = x_minus_y
    mulps   %xmm5, %xmm6           # xmm6 = x_minus_y^2

    addps   %xmm7, %xmm6           # xmm6 = pow_x_minus_y + 1

    addps   %xmm4, %xmm0           # sum += xy

    mulps   %xmm6, %xmm1           # mult *= pow_x_minus_y_plus_1

    addq    $16, %rdi              # px += 4
    addq    $16, %rsi              # py += 4

    incq    %r8

    jmp     .LoopStart

.END_LOOP:

    # the final sum is spread among the 4 indices, we sum them all
    movaps  %xmm0, %xmm7           
    shufps  $0xB1, %xmm0, %xmm7    
    addps   %xmm7, %xmm0          

    # same with mul
    movaps  %xmm0, %xmm7         
    shufps  $0x4E, %xmm0, %xmm7 
    addps   %xmm7, %xmm0          

    # first element of vector as float to 64 bit int, to check result in gdb
    # cvttss2siq %xmm0, %rax

    movaps  %xmm1, %xmm7         
    shufps  $0xB1, %xmm1, %xmm7   
    mulps   %xmm7, %xmm1           

    movaps  %xmm1, %xmm7           
    shufps  $0x4E, %xmm1, %xmm7    
    mulps   %xmm7, %xmm1          

    # cvttss2siq %xmm1, %rax

    # compute result
    movss   %xmm1, %xmm7           # xmm7 = mult[0]
    divss   %xmm7, %xmm0           # xmm0 = sum[0] / mult[0]
    # sum[0] as output
    cvttss2siq %xmm0, %rax

    popq    %rbp
    ret
