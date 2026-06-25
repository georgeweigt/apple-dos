asm: defs.h prototypes.h *.c
	$(CC) -Wall -O0 -o asm -include defs.h -include prototypes.h *.c

prototypes.h: *.c
	make -s -C ../tools make-prototypes
	../tools/make-prototypes *.c > prototypes.h
