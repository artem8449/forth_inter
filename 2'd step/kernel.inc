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
	
	