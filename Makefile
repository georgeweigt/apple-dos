.PHONY: check clean all

asm: asm.c
	$(CC) -Wall -O0 -o asm asm.c

asm.c: src/LICENSE src/defs.h src/prototypes.h src/*.c
	cat src/LICENSE src/defs.h src/prototypes.h src/*.c > asm.c

check: asm
	./asm apple-dos.s "Apple DOS 3.3 January 1983.dsk"

clean:
	rm -f asm apple-dos-orig.s patchfile

all:
	make -C src prototypes.h
	make asm
	make apple-dos-orig.s
	diff -u apple-dos-orig.s apple-dos.s > patchfile || : # force success

SRC =	Apple\ DOS\ 3.3C\ Source\ Code/DOS33C.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/RELOCTR.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/DOSINIT.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/DOSHOOK.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/CMDSCAN.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/XOPNCLS.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/XLODSAV.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/XMISCMD.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/DOSGOER.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/BLDFTAB.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/CMDTBLS.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/FDOSENT.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/FOPCLRW.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/FDELCAT.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/FMTRWIO.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/FLOCNXB.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/FLOCSEC.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/FVCBUFS.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/BOOTLDR.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/COREQUS.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/PRENIBL.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/WRITRTN.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/POSTNRD.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/RDADSEK.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/MSWAITR.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/WRITADR.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/RWTSONE.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/RWTSTWO.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/FORMATR.pretty \
	Apple\ DOS\ 3.3C\ Source\ Code/DOSPTCH.pretty

apple-dos-orig.s: $(SRC)
	cat  $(SRC) > apple-dos-orig.s || :
