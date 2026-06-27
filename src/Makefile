asm: defs.h prototypes.h *.c
	$(CC) -Wall -O0 -o asm -include defs.h -include prototypes.h *.c

prototypes.h: *.c ../tools/make-prototypes
	../tools/make-prototypes *.c > prototypes.h

../tools/make-prototypes: ../tools/make-prototypes.c
	make -C ../tools make-prototypes
