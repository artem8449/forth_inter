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


;colon words such utils (I/O working, parser)
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

;make place to sym and ret adr 
colon  "stack-cell", stack_cell
	dq xt_lit, 0, xt_sp, xt_exit
	
; ( fd - char 1) or ( - 0 ) read char from stdin 
colon "file-read-char", file_read_char
  dq xt_to_r, xt_stack_cell, xt_from_r, xt_swap, xt_lit, 1, xt_sys_read
	branch0 .fail
  dq xt_lit, 1
.fail :
	dq xt_exit
	
; ( fd char - ) print char to screan 
colon "file-write-char", file_write_char
  dq xt_swap, xt_to_r, xt_to_r
  dq xt_stack_cell, xt_dup, xt_from_r, xt_swap, xt_save_char
  dq xt_from_r, xt_swap, xt_lit, 1, xt_sys_write, xt_drop, xt_drop, xt_exit

colon "string-length", string_length
      dq xt_dup
.loop:
      dq xt_dup, xt_fetch_char
      branch0 .exit
      dq xt_inc
      branch .loop
.exit:
	  dq xt_swap, xt_sub, xt_exit
  
; print carriage return sym
colon "print-cr", print_cr
	dq xt_lit, 1, xt_lit, 10, xt_file_write_char, xt_exit

;  ( str count - ) print count in str 
colon "print-string-counted", print_string_counted
	dq xt_to_r, xt_to_r, xt_lit, 1, xt_from_r, xt_from_r, xt_sys_write, xt_drop, xt_exit

; ( str -- ) count str length and print str 
colon "print-string", print_string
	dq xt_dup, xt_string_length, xt_print_string_counted, xt_exit

;check sym to white space (whith tab sym)	
colon "char-is-space", char_is_space
  dq xt_to_r
  dq xt_r_fetch, xt_lit,  ' ', xt_equal
  dq xt_r_fetch, xt_lit, 10, xt_equal, xt_lor
  dq xt_r_fetch, xt_lit, 13, xt_equal, xt_lor
  dq xt_r_fetch, xt_lit, 9, xt_equal, xt_lor
  dq xt_from_r,  xt_lit, '\t', xt_equal, xt_lor, xt_exit
	
; ( addr - ) add 0 terminate to str 
colon "string-terminate", string_terminate
	dq xt_lit, 0, xt_swap, xt_save_char, xt_exit
	
;  ( fd buffer - ) word to read word whith check space 
colon "file-read-word", file_read_word
	dq xt_dup, xt_to_r, xt_to_r

;miss all unusual space 
.read_space:
	  dq xt_dup, xt_file_read_char
	  branch0 .eof

	  dq xt_dup, xt_char_is_space
	  branch0 .commit

	  dq xt_drop
	  branch .read_space

;read unspace sym 
.read:
	  dq xt_dup, xt_file_read_char
	  branch0 .eof

	  dq xt_dup, xt_char_is_space, xt_not
	  branch0 .end
;save now sym and go to next 
.commit:
	  dq xt_r_fetch, xt_save_char
	  dq xt_from_r, xt_inc, xt_to_r
	  branch .read

.end:
	dq xt_drop
;add 0 terminate to str 
.eof:
	  dq xt_drop, xt_r_fetch, xt_string_terminate
	  dq xt_from_r, xt_from_r, xt_sub, xt_exit

;save up of stack to mem where saved new word, and go to next token 	  
colon ",", comma
	  dq xt_here, xt_fetch, xt_save
	  dq xt_here, xt_fetch, xt_lit, 8, xt_plus
	  dq xt_here, xt_save, xt_exit
;save byte up of stack to mem where saved new word
colon "c,", char_comma
	  dq xt_here, xt_fetch, xt_save_char
	  dq xt_here, xt_fetch, xt_inc
	  dq xt_here, xt_save, xt_exit

;double call dup or inc 	  
colon "2dup", ddup
	dq xt_to_r, xt_dup, xt_r_fetch, xt_swap, xt_from_r, xt_exit

colon "2inc", dinc
	dq xt_to_r, xt_inc, xt_from_r, xt_inc, xt_exit

;compare strs and ret 1 (yes) or 0 (no)	
colon "string-equals", string_equals
.start:
	  dq xt_ddup, xt_fetch_char, xt_swap, xt_fetch_char, xt_equal
	  branch0 .no
	  dq xt_dup, xt_fetch_char
	  branch0 .yes
	  dq xt_dinc
	  branch .start
.yes:
	dq xt_drop, xt_drop, xt_lit, 1, xt_exit
.no:
	dq xt_drop, xt_drop, xt_lit, 0, xt_exit
  
  ; ( dest source - ) copy str to adr o up of stack
colon "string-copy", string_copy
	.loop:
		dq xt_ddup, xt_fetch_char, xt_to_r, xt_r_fetch, xt_swap, xt_save_char
		dq xt_dinc, xt_from_r, xt_not
	branch0 .loop
				dq xt_drop, xt_drop, xt_exit

;read colon word in compile state of machine and save them 				
colon ":", colon
  .restart:
		dq xt_fd, xt_fetch, xt_lit, word_buf, xt_file_read_word
		branch0 .restart
		dq xt_lit, 0, xt_lit, word_buf, xt_forth_create
		dq xt_lit, STATE_COMPILE, xt_state, xt_save
		dq xt_lit, i_docol, xt_comma, xt_exit

;exit from compile mod to  interpretator 
colon ";", semicolon, 1
		dq xt_lit, STATE_INTERPRET, xt_state, xt_save
		dq xt_lit, xt_exit, xt_comma, xt_exit

;check char to digit save flag 		
colon "char-is-digit", char_is_digit
	  dq xt_to_r
	  dq xt_r_fetch, xt_lit, '9'+1, xt_less
	  dq xt_lit, '0'-1, xt_r_fetch, xt_less, xt_land
	  dq xt_from_r, xt_drop, xt_exit

;go tp iz_zero if and of up of stack 0	  
colon "drop0", drop0
	  dq xt_dup
	  branch0 .is_zero
	  dq xt_exit
	  .is_zero:
	  dq xt_drop, xt_exit

;convert ascii code to digit 
colon "char-to-digit", char_to_digit
	  dq xt_lit, '0', xt_sub, xt_exit
	
; ( buf -- num? len ) 
colon "string-parse-unsigned-number", string_parse_unsigned_number
	  dq xt_dup, xt_to_r
	  dq xt_lit, 0, xt_to_r
.loop:
	  dq xt_dup, xt_fetch_char, xt_dup, xt_char_is_digit
	  branch0 .end
	  dq xt_char_to_digit
	  dq xt_lit, 10, xt_from_r, xt_mul, xt_plus, xt_to_r
	  dq xt_inc
	  branch .loop
  .end:
	  dq xt_drop, xt_from_r, xt_swap, xt_from_r, xt_sub, xt_drop0, xt_exit

;check that num less than 0
colon "neg", neg
	  dq xt_lit, 0, xt_swap, xt_sub, xt_exit

;increment some digit on mem 
colon "inc", inc
      dq xt_lit, 1, xt_plus, xt_exit  

; ( buf -- num? len ) check type of char num and parse 
colon "string-parse-number", string_parse_number
	  dq xt_dup, xt_fetch_char, xt_dup, xt_char_is_digit, xt_not
	  branch0 .unsigned
	  dq xt_dup, xt_lit, '-', xt_equal, xt_not
	  branch0 .negative

	  dq xt_dup, xt_lit, '+', xt_equal, xt_not
	  branch0 .positive

.negative:
	  dq xt_drop
	  dq xt_inc, xt_string_parse_unsigned_number
	  dq xt_dup
	  branch0 .error
	  dq xt_inc
	  dq xt_swap, xt_neg, xt_swap, xt_exit
.positive:
	  dq xt_drop
	  dq xt_inc, xt_string_parse_unsigned_number
	  dq xt_dup
  branch0 .error
	  dq xt_inc, xt_exit
.unsigned:
	  dq xt_drop, xt_string_parse_unsigned_number, xt_exit 
.error:
      dq xt_drop, xt_lit, 0, xt_exit	


;module to forth interpretator and words to work with dictinory 

	  