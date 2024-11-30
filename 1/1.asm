.section .data

EOS_mask:

.byte 0x1,0xFF
.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0

_format: .string "%d\n"

.section .text
.globl strlens
.globl formula1

strlens:

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


; -----------------------------
; MY CODE ---------------------
; -----------------------------

formula1:

pushq %rbp
movq %rsp, %rbp
                        ; set locals
movq 8(%rsi), %r12      ; str1
movq 16(%rsi), %r13     ; str2
pushq $0                ; diff
                       
movq %r12, %rdi         ; len1
call strlens
movq %rax, %rcx         
                        
movq %r13, %rdi         ; len2
call strlens
movq %rax, %rdx

; push len1, len2
pushq %rcx
pushq %rdx

cmpq %rdx, %rcx         ; abs(len1-len2)
                      
jle .ELSE
subq %rdx, %rcx        
movq %rcx, %rax
.ELSE:
subq %rcx, %rdx       
movq %rdx, %rax

pushq %rax ; push abs(len1-len2) into stack

; restore len1, len2
popq %rdx  ; len2
popq %rcx  ; len1

cmpq %rdx, %rcx         ;  min(len1, len2)/4
cmovg %rdx, %rcx
shrq $2, %rcx
movq %rcx, %r9
                        ; rsp-8:  diff
                        ; rsp:    abs(len1-len2)
                        ; r9:     min(len1,len2)/4
                        ; r11:    counter
                        ; r12:    str1
                        ; r13:    str2
.START:
                    
cmpq %r11, %r9          ; min(len1, len2)/4 - counter
jl .DONE
                        ; SIMD operations
movd (%r12), %xmm0    ; MOVe unaligneD Doubleword - loads 16 bytes from str1
movd (%r13), %xmm1    ; MOVe unaligneD Doubleword - loads 16 bytes from str2

pcmpeqb %xmm1, %xmm0    ; Packed CoMPare for EQual Bytes - compares each byte in xmm0, xmm1
pmovmskb %xmm0, %r10d   ; Packed MOVe MaSK Byte          - bitmask from each byte in result into r10d (lower 32 bits of r10)
not %r10d               ; flip the bitmask
and $0xF, %r10d         ; only get the first 4 bytes
popcnt %r10d, %r10d     ; get the number of turned on bits
addq %r10, 8(%rsp)    ; diff += differences

addq $4, %r12           ; str1 += 4
addq $4, %r13           ; str2 += 4
addq $1, %r11           ; counter ++
jmp .START
                        
.DONE:  
                        
addq %(rsp), 8(%rsp)    ; diff += abs(len1 - len2);

movq $_format, %rdi     ; print difference
movq 8(%rsp), %rsi
call printf
leave
ret