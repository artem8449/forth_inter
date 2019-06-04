section .rodata
msg:
  .no_such_word: db "! No such word: ", 0

section .bss
word_buf:  resq 512
user_dict:  resq 65536
section .data
here:  dq user_dict
fd: dq 0

%define STATE_COMPILE 1
%define STATE_INTERPRET 0
state: dq STATE_INTERPRET

const "forth-here", here
const "forth-last-word", last_word
const "forth-is-compiling", state
const "forth-input-fd", fd
const "forth-stack-start", stack_start


;colon words such utils 
colon "bye", bye
    dq xt_lit, 60
    TIMES 6 dq xt_lit, 0
    dq xt_syscall

%define sys_read_no 0
%define sys_write_no 1

colon "sys-read", sys_read
TIMES 3 dq xt_to_r
dq xt_lit, sys_read_no
TIMES 3 dq xt_from_r
TIMES 3 dq xt_lit, 0
dq xt_syscall , xt_drop, xt_exit

colon "sys-write", sys_write
TIMES 3 dq xt_to_r
dq xt_lit, sys_write_no
TIMES 3 dq xt_from_r
TIMES 3 dq xt_lit, 0
dq xt_syscall , xt_drop, xt_exit

