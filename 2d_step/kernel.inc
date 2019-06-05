;aritmetical operators 
native "+", plus
	pop rax
	add [rsp], rax
	jmp next

native "*", mul
	pop rdx
	pop rax
	imul rdx
	push rax
	jmp next

native "/", div
	pop rcx
	pop rax
	cqo
	idiv rcx
	push rax
	jmp next

native "%", div_rem
	pop rcx
	pop rax
	cqo
	idiv rcx
	push rdx
	jmp next

native "-", sub
	pop rax
	sub [rsp], rax
	jmp next

native "=", equal
  pop rdx
  pop rax
  cmp rax, rdx
  jne .nequals
  xor rax, rax
  mov rax, 1
  push rax
  jmp next
.nequals:
  xor rax, rax
  push rax
  jmp next

native "<", less
	pop rdx
	pop rax
	cmp rax, rdx
	jl .less
	push 0
	jmp next
.less:
	push 1
	jmp next

native ">", greater
	pop rdx
	pop rax
	cmp rax, rdx
	jg .greater
	push 0
	jmp next
.greater:
	push 1
	jmp next
	
;some comands to work with stack
native 'drop', drop
	add rsp, 8
	jmp next

native 'swap', swap
    pop rdx
    pop rax
    push rdx
    push rax
    jmp next
	
native 'dup', dup
	push qword [rsp]
	jmp next
	
;comands to work with returnment stack
native ">r", to_r
    pop rax
    rpush rax
    jmp next

native "r>", from_r
    rpop rax
    push rax
    jmp next

native "r@", r_fetch
    push qword [rstack]
    jmp next
	

;logic operators 
native 'not', not
  pop rax
  test rax, rax
  jz .setone
  xor rax, rax
  push rax
  jmp next
  .setone:
    xor rax, rax
    mov rax, 1
    push rax
    jmp next

native "and", bit_and
	pop rax
	pop rdx
	and rdx, rax
	push rax
	jmp next

native "or", bit_or
	pop rax
	pop rdx
	or rax, rdx
	push rax
	jmp next
	
native 'lor', lor
  pop rax
  pop rdx
  test rax, rax
  jnz .yes
  push rdx
  jmp next
  .yes:
    push rax
    jmp next

native "land", land
	pop rax
	pop rdx
	test rax, rax
	jz .false
	test rdx, rdx
	jz .false
	push 1
	jmp next
.false:
	push 0
	jmp next

;words work with memory
native "@", fetch
    pop rax
    push qword[rax]
    jmp next

native "!", save
    pop rax
    pop rdx
    mov [rax], rdx
    jmp next

native "c!", save_char
    pop rax
    pop rdx
    mov [rax], dl
    jmp next

native "c@", fetch_char
    pop rax
    movzx rax, byte [rax]
    push rax
    jmp next
	
native "forth-dp", forth_dp
	push dp
	jmp next
	
native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]	
	
	
;runtime need comands 
native "docol", docol
    rpush pc
    add w, 8
    mov pc, w
    jmp next
	
native "branch", branch
    mov pc, [pc]
    jmp next

native "0branch", branch0
    pop rax
    test rax, rax
    jnz .skip
    mov pc, [pc]
    jmp next
    .skip:
    add pc, 8
    jmp next

native "exit", exit
    rpop pc
    jmp next
	
native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next
	
native "sp", sp
    push rsp
    jmp next
	
native "forth-stack-restore", forth_stack_restore
  mov rsp, [stack_start]
  jmp next
  
native "forth-stack-base", forth_stack_base
	push qword[stack_start]
	jmp next
	
;( call_num a1 a2 a3 a4 a5 a6 -- new_rax new_rdx )
;rdi, rsi, rdx, r10, r8, r9
native "syscall", syscall
	pop r9
	pop r8
	pop r10
	pop rdx
	pop rsi
	pop rdi
	pop rax
	syscall
	push rax
	push rdx
	jmp next
	

	
	