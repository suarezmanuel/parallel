
// #include <stdio.h>
// #include <emmintrin.h>
// #include <string.h>

int main () {
    __asm__(
        "_format: .string "%d\n""

        "pushq %rbp"
        "movq %rsp, %rbp"

        // set locals
        "pushq 8(%rsi)"
        "pushq 16(%rsi)"
        "pushq $0"

        // len1
        "movq -8(%rbp), %rdi"
        "call strlens"
        "pushq %rax"
        "movq %rax, %rcx"       // rcx: len1
        // len2
        "movq -16(%rbp), %rdi"
        "call strlens"
        "pushq %rax"            // rax: len2

        //  min(len1, len2)/4
        "cmpq %rcx, %rax"
        "cmovg %rcx, %rax"
        "shrq $2, %rax"
        "movq %rax, %r9"
        "movq $0, %r11"

                                // rbp-8:  str1
                                // rbp-16: str2
                                // rbp-24: diff
                                // rbp-32: len1
                                // rbp-40: len2
                                // r9:     min(len1,len2)/4
                                // r11:    counter
        "movq -8(%rbp),  %r12"  // r12:    str1
        "movq -16(%rbp), %r13"  // r13:    str2
        

        ".START:"
                         
        "cmpq %r11, %r9"        // min(len1, len2)/4 - counter
        "jl .DONE"
    
                                // SIMD operations
        "movdqu (%r12), %xmm0"  // MOVe unaligneD QUadword - loads 16 bytes from str1
        "movdqu (%r13), %xmm1"  // MOVe unaligneD QUadword - loads 16 bytes from str2

        "pcmpeqb %xmm1, %xmm0"  // Packed CoMPare for EQual Bytes - compares each byte in xmm0, xmm1
        "pmovmskb %xmm0, %r10d" // Packed MOVe MaSK Byte          - bitmask from each byte in result into r10d (lower 32 bits of r10)
        "not %r10d"             // flip the bitmask
        "and $0xF, %r10d"       // only get the first 4 bytes
        "popcnt %r10d, %r10d"   // get the number of turned on bits
        "addq %r10, -24(%rbp)"  // diff += differences

        "addq $4, %r12"         // str1 += 4
        "addq $4, %r13"         // str2 += 4
        "addq $1, %r11"         // counter ++
        "jmp .START"

                                // loop done
        ".DONE:"
        "movq -32(%rbp), %rdx"  // len1
        "movq -40(%rbp), %rcx"  // len2
        "cmpq %rdx, %rcx"
                                // if len2-len1 > 0
        "jg .ELSE"
        "subq %rcx, %rdx"       // len1 = len1-len2
        "movq %rdx, %rax"
        ".ELSE:"
        "subq %rdx, %rcx"       // len2 = len2-len1
        "movq %rcx, %rax"
                                // diff += abs(len1 - len2);
        "addq %rax, -24(%rbp)"

                                // print difference
        "movq $_format, %rdi"
        "movq -24(%rbp), %rsi"
        "call printf"
        "leave"
        "ret"
    );
}

/*

.section .data
EOS_mask:
.byte 0x1,0xFF
.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0
.section .text
.globl strlens
strlen1:
push %rbp
mov %rsp, %rbp
;Zero our counting registers
xor %rax, %rax
xor %rcx, %rcx
;Load mask into xmm1
lea EOS_mask, %rsi
movdqu (%rsi), %xmm1
.loop:
add %rax, %rcx
pcmpistri $0b00010100, (%rdi,%rax), %xmm1
jnz .loop
add %rax, %rcx
pop %rbp
ret

*/

// int main (int argc, char** argv) {

//     char* str1 = argv[1];
//     char* str2 = argv[2];
//     int diff = 0;

//     int len1 = strlen(str1);
//     int len2 = strlen(str2);

//     for (int i=0; i < min(len1, len2)/4; i++) {
//         __m128i a = _mm_setr_epi8(str1[0],str1[1],str1[2],str1[3],0,0,0,0, 0,0,0,0, 0,0,0,0); 
//         __m128i b = _mm_setr_epi8(str2[0],str2[1],str2[2],str2[3],0,0,0,0, 0,0,0,0, 0,0,0,0); 

//         __m128i result = _mm_cmpeq_epi8(a, b);

//         int mask = _mm_movemask_epi8(result);

//         diff += __builtin_popcount(mask & 0xF);

//         str1 += 4;
//         str2 += 4;
//     }

//     diff += abs(len1 - len2);

//     printf("%d", diff);
// }
