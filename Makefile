asm: asm.c
	$(CC) -Wall -O0 -o asm asm.c

asm.c: src/LICENSE src/defs.h src/prototypes.h src/*.c
	cat src/LICENSE src/defs.h src/prototypes.h src/*.c > asm.c
