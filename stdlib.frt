: here forth-here @ ;
: IMMEDIATE forth-last-word @ forth-cfa 1 - dup c@ 1 or swap c! ;

: if ' 0branch , here 0  , ; IMMEDIATE
: else ' branch , here 0 , swap here swap !  ; IMMEDIATE
: then here swap ! ; IMMEDIATE

