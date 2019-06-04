;aritmetical operators 
native "+", plus
	pop rax
	add [rsp], rax
	jmp next

native "*", mul
	pop rax
	mul qword[rsp]
	pop rdx
	push rax
	jmp next

native "/", div
	pop rcx
	pop rax
	cqo
	div rcx
	push rax
	jmp next

native "%", div_rem
	pop rcx
	pop rax
	cqo
	div rcx
	push rdx
	jmp next

native "-", sub
	pop rax
	sub [rsp], rax
	jmp next

native "=", equal
	pop rax
	pop rdx
	cmp rax, rdx
	je .equals
	push 0
	jmp next
.equals:
	push 1
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