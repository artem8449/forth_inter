global _start
%include "macro.inc"

%define pc r15		;next command pointer
%define w r14		;sub commands pointer
%define rstack r13	;return stack pointer

section .text

%include "kernel.inc"

section .bss

resq 1023				;return stack end
rstack_start: resq 1	;return stack start

user_mem: resq 65536	;global data for user

section .data

last_word: dq _lw	;point to the last word on dictinory 

dp: dq user_mem		;current global data pointer
stack_start: dq 0	;stores a saved address of data stack

section .text

_start:
	mov rstack, rstack_start
	mov [stack_start], rsp

	mov pc, forth_init

next:
	mov w, pc
	add pc, 8
	mov w, [w]
	jmp [w]
