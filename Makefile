ASM         = nasm
ASMFLAGS    = -felf64 -g -Isrc/ 
LINKER      = ld

all: bin/forth_iner
	
bin/forth_iner: objects/forth_iner.o 
	mkdir -p bin 
	$(LINKER) -o bin/forth_iner  $(LINKERFLAGS) -o bin/forth_iner objects/forth_iner.o $(LIBS)

objects/forth_iner.o: 2'd_step/forth_iner.asm 2'd_step/macro.inc 2'd_step/colons.inc 
	mkdir -p objects
	$(ASM) $(ASMFLAGS) 2'd_step/forth_iner.asm -o objects/forth_iner.o

clean: 
	rm -rf bin objects

.PHONY: clean

