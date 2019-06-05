ASM         = nasm
ASM_FLAGS   = -felf64 -g -I2d_step/
LINKER      = ld

all: bin/forth_inter
	
bin/forth_inter: objects/forth_inter.o 
	mkdir -p bin 
	$(LINKER) -o bin/forth_inter  $(LINKERFLAGS) -o bin/forth_inter objects/forth_inter.o $(LIBS)

objects/forth_inter.o: 2d_step/forth_inter.asm 2d_step/macro.inc 2d_step/colons.inc 
	mkdir -p objects
	$(ASM) $(ASM_FLAGS) 2d_step/forth_inter.asm -o objects/forth_inter.o

clean: 
	rm -rf bin objects

.PHONY: clean

