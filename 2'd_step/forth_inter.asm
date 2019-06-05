global _start
%include "macro.inc"

%define pc r15		;next command pointer
%define w r14		;sub commands pointer
%define rstack r13	;return stack pointer


section .bss

resq 1023		;end of return stack 
rstack_start: resq 1	;start of return stack 

user_mem: resq 65536	;set global data for user

section .data

last_word: dq _lw	;point to the last word on dictinory 

dp: dq user_mem		;current global data pointer
stack_start: dq 0	;stores a saved address of data stack

section .text

%include "kernel.inc"
%include "colons.inc"

_start:
	mov rstack, rstack_start
	mov [stack_start], rsp

	mov pc, forth_init

next:
	mov w, pc
	add pc, 8
	mov w, [w]
	jmp [w]
