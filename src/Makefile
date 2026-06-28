.PHONY: check

asm: defs.h prototypes.h *.c
	$(CC) -Wall -O0 -o asm -include defs.h -include prototypes.h *.c

prototypes.h: *.c
	make -s -C ../tools make-prototypes
	../tools/make-prototypes *.c > prototypes.h

check:
	make -s -C ../tools wcheck
	../tools/wcheck *.c
