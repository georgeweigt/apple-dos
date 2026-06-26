.PHONY: all check

asm: asm.c
	$(CC) -Wall -O0 -o asm asm.c

asm.c: src/LICENSE src/defs.h src/prototypes.h src/*.c
	cat src/LICENSE src/defs.h src/prototypes.h src/*.c > asm.c

all:
	make -C src prototypes.h
	make asm

check:
	./asm apple-dos.s "Apple DOS 3.3 January 1983.dsk"
