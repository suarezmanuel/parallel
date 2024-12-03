.section .data

.align 16
# Define a constant vector with all elements set to 1.0f
_1_ps:
    .float 1.0, 1.0, 1.0, 1.0

.section .text
.globl function
# .globl main

# main:

function:
    # Function prologue
    pushq   %rbp
    movq    %rsp, %rbp

    # Initialize sum to zero (__m128 sum = _mm_set_ps1(0);)
    xorps   %xmm0, %xmm0           # xmm0 = 0.0, sum

    # Load a vector of ones (__m128 _1 = _mm_set_ps1(1);)
    movaps  _1_ps(%rip), %xmm7     # xmm1 = {1.0, 1.0, 1.0, 1.0}, mult
    movaps  %xmm7, %xmm1     # xmm1 = {1.0, 1.0, 1.0, 1.0}, mult

    # Prepare loop variables
    movq    %rdx, %rcx             # rcx = n
    shrq    $2, %rcx               # rcx = n / 4 (number of SIMD iterations)
    xorq    %r8, %r8               # r8 = 0 (loop counter)

    # Pointers to px and py are in %rdi and %rsi respectively

    # Start of the loop
.LoopStart:
    cmpq    %r8, %rcx              # Compare loop counter with iteration count
    jle     .LoopEnd               # If r8 >= rcx, exit loop

    # Load x and y vectors (__m128 x = _mm_loadu_ps(px);)
    movups  (%rdi), %xmm2          # xmm2 = x = *px
    movups  (%rsi), %xmm3          # xmm3 = y = *py

    # Compute xy = x * y (__m128 xy = _mm_mul_ps(x, y);)
    movaps  %xmm2, %xmm4           # xmm4 = x
    mulps   %xmm3, %xmm4           # xmm4 = x * y

    # Compute x_minus_y = x - y (__m128 x_minus_y = _mm_sub_ps(x, y);)
    movaps  %xmm2, %xmm5           # xmm5 = x
    subps   %xmm3, %xmm5           # xmm5 = x - y

    # Compute pow_x_minus_y = (x_minus_y) * (x_minus_y); (__m128 pow_x_minus_y = _mm_mul_ps(x_minus_y, x_minus_y);)
    movaps  %xmm5, %xmm6           # xmm6 = x_minus_y
    mulps   %xmm5, %xmm6           # xmm6 = x_minus_y^2

    # Compute pow_x_minus_y_plus_1 = pow_x_minus_y + 1; (__m128 pow_x_minus_y_plus_1 = _mm_add_ps(pow_x_minus_y, _1);)
    addps   %xmm7, %xmm6           # xmm6 = pow_x_minus_y + 1

    # Update sum (__m128 sum = _mm_add_ps(sum, xy);)
    addps   %xmm4, %xmm0           # sum += xy

    # Update mult (__m128 mult = _mm_mul_ps(mult, pow_x_minus_y_plus_1);)
    mulps   %xmm6, %xmm1           # mult *= pow_x_minus_y_plus_1

    # Advance pointers px and py by 16 bytes (4 floats)
    addq    $16, %rdi              # px += 4
    addq    $16, %rsi              # py += 4

    # Increment loop counter
    incq    %r8

    # Loop back
    jmp     .LoopStart

# End of the loop
.LoopEnd:
    # Reduction of sum vector to a scalar

    # First shuffle and add (sum = sum + shuffle(sum, sum, _MM_SHUFFLE(2,3,0,1));)
    movaps  %xmm0, %xmm7           # xmm7 = sum
    shufps  $0xB1, %xmm0, %xmm7    # xmm7 = shuffle(sum, sum, 0xB1)
    addps   %xmm7, %xmm0           # sum += shuffled sum

    # Second shuffle and add (sum = sum + shuffle(sum, sum, _MM_SHUFFLE(1,0,3,2));)
    movaps  %xmm0, %xmm7           # xmm7 = sum
    shufps  $0x4E, %xmm0, %xmm7    # xmm7 = shuffle(sum, sum, 0x4E)
    addps   %xmm7, %xmm0           # sum += shuffled sum

    # Reduction of mult vector to a scalar

    cvttss2siq %xmm0, %rax

    # First shuffle and multiply (mult = mult * shuffle(mult, mult, _MM_SHUFFLE(2,3,0,1));)
    movaps  %xmm1, %xmm7           # xmm7 = mult
    shufps  $0xB1, %xmm1, %xmm7    # xmm7 = shuffle(mult, mult, 0xB1)
    mulps   %xmm7, %xmm1           # mult *= shuffled mult

    # Second shuffle and multiply (mult = mult * shuffle(mult, mult, _MM_SHUFFLE(1,0,3,2));)
    movaps  %xmm1, %xmm7           # xmm7 = mult
    shufps  $0x4E, %xmm1, %xmm7    # xmm7 = shuffle(mult, mult, 0x4E)
    mulps   %xmm7, %xmm1           # mult *= shuffled mult

    # Extract the scalar values from sum and mult

    cvttss2siq %xmm1, %rax

    # Compute the division (result = sum[0] / mult[0];)
    movss   %xmm1, %xmm7           # xmm7 = mult[0]
    divss   %xmm7, %xmm0           # xmm0 = sum[0] / mult[0]

    # Function return value is in xmm0
    # Function epilogue
    popq    %rbp
    ret
