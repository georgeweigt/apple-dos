/*
BSD 2-Clause License

Copyright (c) 2026, George Weigt
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define NSYM 200
#define STACKSIZE 100
#define TOKENBUFLEN 100

#define TRACE printf("%s %d\n", __FUNCTION__, __LINE__);

struct sym {
	char *name;
	int value;
	int where; // line number where symbol defined
};

extern char *buf;
extern int token;
extern int tokenlen;
extern char tokenbuf[TOKENBUFLEN + 1];
extern char *scanptr;
extern char *tokenptr;
extern char *lineptr;
extern int addrmode;
extern int value;
extern int pass;
extern int where;
extern struct sym stab[26][NSYM];
extern int stack[STACKSIZE];
extern int stackindex;
extern int curloc;
extern int curlin;
extern int lstloc;
extern int cmp_count;
extern int err_count;
extern uint8_t *mem;
extern uint8_t *disk;

#define T_LABEL  1001
#define T_NAME   1002
#define T_DECSTR 1003
#define T_HEXSTR 1004
#define T_QUOSTR 1005
#define T_SHR    1006
#define T_SHL    1007
#define T_LF     1008

#define AM_IMM   1
#define AM_ZP    2
#define AM_ZPXI  3
#define AM_ZPIY  4
#define AM_ABS   5
#define AM_ABSX  6
#define AM_ABSY  7
#define AM_REGA  8

#define ORA_ZPXI 0x01
#define ORA_ZP   0x05
#define ORA_IMM  0x09
#define ORA_ABS  0x0d
#define ORA_ZPIY 0x11
#define ORA_ZPX  0x15
#define ORA_ABSY 0x19
#define ORA_ABSX 0x1d

#define AND_ZPXI 0x21
#define AND_ZP   0x25
#define AND_IMM  0x29
#define AND_ABS  0x2d
#define AND_ZPIY 0x31
#define AND_ZPX  0x35
#define AND_ABSY 0x39
#define AND_ABSX 0x3d

#define EOR_ZPXI 0x41
#define EOR_ZP   0x45
#define EOR_IMM  0x49
#define EOR_ABS  0x4d
#define EOR_ZPIY 0x51
#define EOR_ZPX  0x55
#define EOR_ABSY 0x59
#define EOR_ABSX 0x5d

#define ADC_ZPXI 0x61
#define ADC_ZP   0x65
#define ADC_IMM  0x69
#define ADC_ABS  0x6d
#define ADC_ZPIY 0x71
#define ADC_ZPX  0x75
#define ADC_ABSY 0x79
#define ADC_ABSX 0x7d

#define STA_ZPXI 0x81
#define STA_ZP   0x85
#define STA_ABS  0x8d
#define STA_ZPIY 0x91
#define STA_ZPX  0x95
#define STA_ABSY 0x99
#define STA_ABSX 0x9d

#define LDA_ZPXI 0xa1
#define LDA_ZP   0xa5
#define LDA_IMM  0xa9
#define LDA_ABS  0xad
#define LDA_ZPIY 0xb1
#define LDA_ZPX  0xb5
#define LDA_ABSY 0xb9
#define LDA_ABSX 0xbd

#define CMP_ZPXI 0xc1
#define CMP_ZP   0xc5
#define CMP_IMM  0xc9
#define CMP_ABS  0xcd
#define CMP_ZPIY 0xd1
#define CMP_ZPX  0xd5
#define CMP_ABSY 0xd9
#define CMP_ABSX 0xdd

#define SBC_ZPXI 0xe1
#define SBC_ZP   0xe5
#define SBC_IMM  0xe9
#define SBC_ABS  0xed
#define SBC_ZPIY 0xf1
#define SBC_ZPX  0xf5
#define SBC_ABSY 0xf9
#define SBC_ABSX 0xfd

#define ASL_ZP   0x06
#define ASL_REGA 0x0a
#define ASL_ABS  0x0e
#define ASL_ZPX  0x16
#define ASL_ABSX 0x1e

#define ROL_ZP   0x26
#define ROL_REGA 0x2a
#define ROL_ABS  0x2e
#define ROL_ZPX  0x36
#define ROL_ABSX 0x3e

#define LSR_ZP   0x46
#define LSR_REGA 0x4a
#define LSR_ABS  0x4e
#define LSR_ZPX  0x56
#define LSR_ABSX 0x5e

#define ROR_ZP   0x66
#define ROR_REGA 0x6a
#define ROR_ABS  0x6e
#define ROR_ZPX  0x76
#define ROR_ABSX 0x7e

#define STX_ZP   0x86
#define STX_ABS  0x8e
#define STX_ZPX  0x96

#define LDX_IMM  0xa2
#define LDX_ZP   0xa6
#define LDX_ABS  0xae
#define LDX_ZPX  0xb6
#define LDX_ABSX 0xbe

#define DEC_ZP   0xc6
#define DEC_ABS  0xce
#define DEC_ZPX  0xd6
#define DEC_ABSX 0xde

#define INC_ZP   0xe6
#define INC_ABS  0xee
#define INC_ZPX  0xf6
#define INC_ABSX 0xfe

#define STY_ZP   0x84
#define STY_ABS  0x8c
#define STY_ZPX  0x94

#define LDY_IMM  0xa0
#define LDY_ZP   0xa4
#define LDY_ABS  0xac
#define LDY_ZPX  0xb4
#define LDY_ABSX 0xbc

#define CPY_IMM  0xc0
#define CPY_ZP   0xc4
#define CPY_ABS  0xcc

#define CPX_IMM  0xe0
#define CPX_ZP   0xe4
#define CPX_ABS  0xec

#define BIT_ZP   0x24
#define BIT_ABS  0x2c

#define OP_BPL   0x10
#define OP_BMI   0x30
#define OP_BVC   0x50
#define OP_BVS   0x70
#define OP_BCC   0x90
#define OP_BCS   0xb0
#define OP_BNE   0xd0
#define OP_BEQ   0xf0

#define OP_PHP   0x08
#define OP_PLP   0x28
#define OP_PHA   0x48
#define OP_PLA   0x68
#define OP_DEY   0x88
#define OP_TAY   0xa8
#define OP_INY   0xc8
#define OP_INX   0xe8

#define OP_BRK   0x00
#define OP_JSR   0x20
#define OP_RTI   0x40
#define OP_RTS   0x60

#define OP_JMP   0x4c
#define OP_JMPI  0x6c

#define OP_CLC   0x18
#define OP_SEC   0x38
#define OP_CLI   0x58
#define OP_SEI   0x78
#define OP_TYA   0x98
#define OP_CLV   0xb8
#define OP_CLD   0xd8
#define OP_SED   0xf8

#define OP_TXA   0x8a
#define OP_TXS   0x9a
#define OP_TAX   0xaa
#define OP_TSX   0xba
#define OP_DEX   0xca
#define OP_NOP   0xea

#define ZPADDR (where <= curlin && value < 256)

#define UNDEF 1000000

#define N 0x80
#define V 0x40
#define E 0x20
#define B 0x10
#define D 0x08
#define I 0x04
#define Z 0x02
#define C 0x01
void emit(int opcode, int count);
void emit_byte(int byte);
void list(void);
int main(int argc, char *argv[]);
void scan_org(struct sym *p);
void scan_equ(struct sym *p);
void scan_asc(void);
void scan_dci(void);
void scan_db(void);
void scan_dw(void);
void scan_ddb(void);
void scan_ds(void);
char * readfile(char *filename);
void scan_file(int k);
void scan_line(void);
struct sym * scan_add_symbol(void);
void scan_addr(void);
void scan_expr(void);
void scan_term(void);
void scan_factor(void);
void scan_error(char *errmsg);
struct sym * scan_lookup(void);
void scan_token(void);
void scan_branch(void);
void scan_value(void);
void scan_adc(void);
void scan_and(void);
void scan_asl(void);
void scan_bcc(void);
void scan_bcs(void);
void scan_beq(void);
void scan_bit(void);
void scan_bmi(void);
void scan_bne(void);
void scan_bpl(void);
void scan_brk(void);
void scan_bvc(void);
void scan_bvs(void);
void scan_clc(void);
void scan_cld(void);
void scan_cli(void);
void scan_clv(void);
void scan_cmp(void);
void scan_cpx(void);
void scan_cpy(void);
void scan_dec(void);
void scan_dex(void);
void scan_dey(void);
void scan_eor(void);
void scan_inc(void);
void scan_inx(void);
void scan_iny(void);
void scan_jmp(void);
void scan_jsr(void);
void scan_lda(void);
void scan_ldx(void);
void scan_ldy(void);
void scan_lsr(void);
void scan_nop(void);
void scan_ora(void);
void scan_pha(void);
void scan_php(void);
void scan_pla(void);
void scan_plp(void);
void scan_rol(void);
void scan_ror(void);
void scan_rti(void);
void scan_rts(void);
void scan_sbc(void);
void scan_sec(void);
void scan_sed(void);
void scan_sei(void);
void scan_sta(void);
void scan_stx(void);
void scan_sty(void);
void scan_tax(void);
void scan_tay(void);
void scan_tsx(void);
void scan_txa(void);
void scan_txs(void);
void scan_tya(void);
void stack_push(int value);
int stack_pop(void);
void stack_add(void);
void stack_sub(void);
void stack_neg(void);
void stack_mul(void);
void stack_div(void);
void stack_rem(void);
void stack_and(void);
void stack_or(void);
void stack_xor(void);
void stack_cpl(void);
void stack_shr(void);
void stack_shl(void);
void stack_lo(void);
void stack_hi(void);
void
emit(int opcode, int count)
{
	emit_byte(opcode);

	if (count > 1)
		emit_byte(value);

	if (count > 2)
		emit_byte(value >> 8);
}

void
emit_byte(int byte)
{
	int index;

	if (lstloc < 0)
		lstloc = curloc;

	if (pass == 1) {
		curloc++;
		return;
	}

	byte &= 0xff;

	if (disk) {
		if (curloc >= 0x1b00 && curloc < 0x3600)
			index = curloc - 0x1b00 + 2560; // track 0, sector 10
		else if (curloc >= 0x3600 && curloc < 0x4000)
			index = curloc - 0x3600; // track 0, sector 0
		else {
			printf("address %04x is out of range\n", curloc);
			exit(1);
		}
		if (disk[index] != byte) {
			printf("compare error at %04x expected %02x have %02x\n", curloc, disk[index], byte);
			err_count++;
		}
		cmp_count++;
	}

	mem[curloc++] = byte;
}
char *buf;
int token;
int tokenlen;
char tokenbuf[TOKENBUFLEN + 1];
char *scanptr;
char *tokenptr;
char *lineptr;
int addrmode;
int value;
int pass;
int where;
struct sym stab[26][NSYM];
int stack[STACKSIZE];
int stackindex;
int curloc;
int curlin;
int lstloc;
int cmp_count;
int err_count;
uint8_t *mem;
uint8_t *disk;
void
list(void)
{
	int i;
	char *s;

	if (lstloc < 0) {
		for (i = 0; i < 16; i++)
			putchar(' ');
		s = lineptr;
		while (*s && *s != '\n')
			putchar(*s++);
		putchar('\n');
		return;
	}

	printf("%04x", lstloc);
	for (i = 0; i < 3; i++) {
		if (lstloc < curloc)
			printf(" %02x", mem[lstloc++]);
		else
			printf("   ");
	}
	printf("   ");

	s = lineptr;
	while (*s && *s != '\n')
		putchar(*s++);
	putchar('\n');

	while (lstloc < curloc) {
		printf("%04x", lstloc);
		for (i = 0; i < 3; i++) {
			printf(" %02x", mem[lstloc++]);
			if (lstloc == curloc)
				break;
		}
		putchar('\n');
	}
}
int
main(int argc, char *argv[])
{
	switch (argc) {
	case 2:
		break;
	case 3:
		disk = (uint8_t *) readfile(argv[2]);
		if (disk == NULL) {
			printf("error reading %s\n", argv[2]);
			exit(1);
		}
		break;
	default:
		exit(1);
	}

	buf = readfile(argv[1]);

	if (buf == NULL) {
		printf("error reading %s\n", argv[1]);
		exit(1);
	}

	mem = malloc(65536);

	if (mem == NULL)
		exit(1);

	scan_file(1);
	scan_file(2);

	if (disk) {
		printf("%d bytes compared\n", cmp_count);
		printf("%d compare errors\n", err_count);
	}
}
void
scan_org(struct sym *p)
{
	int t = pass;
	pass = 2; // no undefined symbols

	scan_token();
	scan_value();

	if (p)
		p->value = value;

	curloc = value;

	pass = t;
}

void
scan_equ(struct sym *p)
{
	int t = pass;
	pass = 2; // no undefined symbols

	scan_token();
	scan_value();

	if (p)
		p->value = value;

	pass = t;
}

// like db except bit 7 is set in strings

void
scan_asc(void)
{
	char *s;
	do {
		scan_token();
		if (token == T_QUOSTR) {
			s = tokenbuf;
			while (*s)
				emit_byte(*s++ | 0x80);
			scan_token();
		} else {
			scan_value();
			emit_byte(value);
		}
	} while (token == ',');
}

// like db except bit 7 is set in the last char of strings

void
scan_dci(void)
{
	char *s;
	do {
		scan_token();
		if (token == T_QUOSTR) {
			s = tokenbuf;
			while (s[0] && s[1])
				emit_byte(*s++);
			if (*s)
				emit_byte(*s | 0x80); // last char
			scan_token();
		} else {
			scan_value();
			emit_byte(value);
		}
	} while (token == ',');
}

// define bytes

void
scan_db(void)
{
	char *s;
	do {
		scan_token();
		// if 1 char then it can be used in an arithmetic expression
		if (token == T_QUOSTR && tokenlen != 1) {
			s = tokenbuf;
			while (*s)
				emit_byte(*s++);
			scan_token();
		} else {
			scan_value();
			emit_byte(value);
		}
	} while (token == ',');
}

// define words

void
scan_dw(void)
{
	do {
		scan_token();
		scan_value();
		emit_byte(value);
		emit_byte(value >> 8);
	} while (token == ',');
}

// like dw except emit high byte first

void
scan_ddb(void)
{
	do {
		scan_token();
		scan_value();
		emit_byte(value >> 8);
		emit_byte(value);
	} while (token == ',');
}

// define storage

void
scan_ds(void)
{
	int i, n, t = pass;
	pass = 2; // no undefined symbols

	scan_token(); // skip 'ds'
	scan_value();

	if (token != ',') {
		curloc += value;
		pass = t;
		return;
	}

	n = value;

	scan_token(); // skip ','
	scan_value();

	pass = t;

	for (i = 0; i < n; i++)
		emit_byte(value);
}
char *
readfile(char *filename)
{
	int fd, n;
	char *buf;
	off_t t;

#ifndef O_BINARY
#define O_BINARY 0
#endif

	fd = open(filename, O_RDONLY | O_BINARY);

	if (fd < 0)
		return NULL;

	t = lseek(fd, 0, SEEK_END);

	if (t < 0 || t > 1000000) {
		close(fd);
		return NULL;
	}

	if (lseek(fd, 0, SEEK_SET)) {
		close(fd);
		return NULL;
	}

	n = (int) t;

	buf = malloc(n + 1);

	if (buf == NULL) {
		close(fd);
		return NULL;
	}

	if (read(fd, buf, n) != n) {
		free(buf);
		close(fd);
		return NULL;
	}

	close(fd);

	buf[n] = '\0';

	return buf;
}
void
scan_file(int k)
{
	pass = k;

	curlin = 1;

	scanptr = buf;

	while (*scanptr) {
		lstloc = -1;
		lineptr = scanptr;
		scan_line();
		if (token != T_LF)
			scan_error("Unexpected text at end of line");
		curlin++;
		if (pass == 2 && disk == NULL)
			list();
	}
}

void
scan_line(void)
{
	int err = 0, i;
	struct sym *p = NULL;

	// comment line?

	if (*scanptr == '*') {
		while (*scanptr && *scanptr != '\n')
			scanptr++;
		token = T_LF;
		if (*scanptr == '\n')
			scanptr++;
		return;
	}

	scan_token();

	if (token == T_LF)
		return;

	if (token == T_LABEL) {
		if (pass == 1) {
			p = scan_add_symbol();
			p->value = curloc;
			p->where = curlin;
		}
		scan_token();
		if (token == T_LF)
			return;
	}

	for (i = 0; i < tokenlen; i++)
		tokenbuf[i] = tolower(tokenbuf[i]);

	switch (*tokenbuf) {

	case 'a':
		if (strcmp(tokenbuf, "adc") == 0) {
			scan_adc();
			break;
		}
		if (strcmp(tokenbuf, "and") == 0) {
			scan_and();
			break;
		}
		if (strcmp(tokenbuf, "asc") == 0) {
			scan_asc();
			break;
		}
		if (strcmp(tokenbuf, "asl") == 0) {
			scan_asl();
			break;
		}
		err = 1;
		break;

	case 'b':
		if (strcmp(tokenbuf, "bcc") == 0) {
			scan_bcc();
			break;
		}
		if (strcmp(tokenbuf, "bcs") == 0) {
			scan_bcs();
			break;
		}
		if (strcmp(tokenbuf, "beq") == 0) {
			scan_beq();
			break;
		}
		if (strcmp(tokenbuf, "bit") == 0) {
			scan_bit();
			break;
		}
		if (strcmp(tokenbuf, "bmi") == 0) {
			scan_bmi();
			break;
		}
		if (strcmp(tokenbuf, "bne") == 0) {
			scan_bne();
			break;
		}
		if (strcmp(tokenbuf, "bpl") == 0) {
			scan_bpl();
			break;
		}
		if (strcmp(tokenbuf, "brk") == 0) {
			scan_brk();
			break;
		}
		if (strcmp(tokenbuf, "bvc") == 0) {
			scan_bvc();
			break;
		}
		if (strcmp(tokenbuf, "bvs") == 0) {
			scan_bvs();
			break;
		}
		err = 1;
		break;

	case 'c':
		if (strcmp(tokenbuf, "clc") == 0) {
			scan_clc();
			break;
		}
		if (strcmp(tokenbuf, "cld") == 0) {
			scan_cld();
			break;
		}
		if (strcmp(tokenbuf, "cli") == 0) {
			scan_cli();
			break;
		}
		if (strcmp(tokenbuf, "clv") == 0) {
			scan_clv();
			break;
		}
		if (strcmp(tokenbuf, "cmp") == 0) {
			scan_cmp();
			break;
		}
		if (strcmp(tokenbuf, "cpx") == 0) {
			scan_cpx();
			break;
		}
		if (strcmp(tokenbuf, "cpy") == 0) {
			scan_cpy();
			break;
		}
		err = 1;
		break;

	case 'd':
		if (strcmp(tokenbuf, "db") == 0 || strcmp(tokenbuf, "dfb") == 0) {
			scan_db();
			break;
		}
		if (strcmp(tokenbuf, "dci") == 0) {
			scan_dci();
			break;
		}
		if (strcmp(tokenbuf, "ddb") == 0) {
			scan_ddb();
			break;
		}
		if (strcmp(tokenbuf, "dec") == 0) {
			scan_dec();
			break;
		}
		if (strcmp(tokenbuf, "dex") == 0) {
			scan_dex();
			break;
		}
		if (strcmp(tokenbuf, "dey") == 0) {
			scan_dey();
			break;
		}
		if (strcmp(tokenbuf, "ds") == 0) {
			scan_ds();
			break;
		}
		if (strcmp(tokenbuf, "dw") == 0) {
			scan_dw();
			break;
		}
		err = 1;
		break;

	case 'e':
		if (strcmp(tokenbuf, "eor") == 0) {
			scan_eor();
			break;
		}
		if (strcmp(tokenbuf, "equ") == 0) {
			scan_equ(p);
			break;
		}
		err = 1;
		break;

	case 'i':
		if (strcmp(tokenbuf, "inc") == 0) {
			scan_inc();
			break;
		}
		if (strcmp(tokenbuf, "inx") == 0) {
			scan_inx();
			break;
		}
		if (strcmp(tokenbuf, "iny") == 0) {
			scan_iny();
			break;
		}
		err = 1;
		break;

	case 'j':
		if (strcmp(tokenbuf, "jmp") == 0) {
			scan_jmp();
			break;
		}
		if (strcmp(tokenbuf, "jsr") == 0) {
			scan_jsr();
			break;
		}
		err = 1;
		break;

	case 'l':
		if (strcmp(tokenbuf, "lda") == 0) {
			scan_lda();
			break;
		}
		if (strcmp(tokenbuf, "ldx") == 0) {
			scan_ldx();
			break;
		}
		if (strcmp(tokenbuf, "ldy") == 0) {
			scan_ldy();
			break;
		}
		if (strcmp(tokenbuf, "lsr") == 0) {
			scan_lsr();
			break;
		}
		err = 1;
		break;

	case 'n':
		if (strcmp(tokenbuf, "nop") == 0) {
			scan_nop();
			break;
		}
		err = 1;
		break;

	case 'o':
		if (strcmp(tokenbuf, "ora") == 0) {
			scan_ora();
			break;
		}
		if (strcmp(tokenbuf, "org") == 0) {
			scan_org(p);
			break;
		}
		err = 1;
		break;

	case 'p':
		if (strcmp(tokenbuf, "pha") == 0) {
			scan_pha();
			break;
		}
		if (strcmp(tokenbuf, "php") == 0) {
			scan_php();
			break;
		}
		if (strcmp(tokenbuf, "pla") == 0) {
			scan_pla();
			break;
		}
		if (strcmp(tokenbuf, "plp") == 0) {
			scan_plp();
			break;
		}
		err = 1;
		break;

	case 'r':
		if (strcmp(tokenbuf, "rol") == 0) {
			scan_rol();
			break;
		}
		if (strcmp(tokenbuf, "ror") == 0) {
			scan_ror();
			break;
		}
		if (strcmp(tokenbuf, "rti") == 0) {
			scan_rti();
			break;
		}
		if (strcmp(tokenbuf, "rts") == 0) {
			scan_rts();
			break;
		}
		err = 1;
		break;

	case 's':
		if (strcmp(tokenbuf, "sbc") == 0) {
			scan_sbc();
			break;
		}
		if (strcmp(tokenbuf, "sec") == 0) {
			scan_sec();
			break;
		}
		if (strcmp(tokenbuf, "sed") == 0) {
			scan_sed();
			break;
		}
		if (strcmp(tokenbuf, "sei") == 0) {
			scan_sei();
			break;
		}
		if (strcmp(tokenbuf, "sta") == 0) {
			scan_sta();
			break;
		}
		if (strcmp(tokenbuf, "stx") == 0) {
			scan_stx();
			break;
		}
		if (strcmp(tokenbuf, "sty") == 0) {
			scan_sty();
			break;
		}
		err = 1;
		break;

	case 't':
		if (strcmp(tokenbuf, "tax") == 0) {
			scan_tax();
			break;
		}
		if (strcmp(tokenbuf, "tay") == 0) {
			scan_tay();
			break;
		}
		if (strcmp(tokenbuf, "tsx") == 0) {
			scan_tsx();
			break;
		}
		if (strcmp(tokenbuf, "txa") == 0) {
			scan_txa();
			break;
		}
		if (strcmp(tokenbuf, "txs") == 0) {
			scan_txs();
			break;
		}
		if (strcmp(tokenbuf, "tya") == 0) {
			scan_tya();
			break;
		}
		err = 1;
		break;

	default:
		err = 1;
		break;
	}

	if (err)
		scan_error("Syntax error");
}

struct sym *
scan_add_symbol(void)
{
	int i, k;
	struct sym *p;

	k = tolower(*tokenbuf) - 'a';

	for (i = 0; i < NSYM; i++) {

		p = &stab[k][i];

		if (p->name == NULL)
			break;

		if (strcmp(p->name, tokenbuf) == 0)
			scan_error("Can't redefine symbol or label");
	}

	if (i == NSYM)
		scan_error("Symbol table full");

	p->name = strdup(tokenbuf);

	if (p->name == NULL)
		scan_error("strdup kaput");

	return p;
}

void
scan_addr(void)
{
	if (token == '#') {
		scan_token();
		scan_value();
		addrmode = AM_IMM;
		return;
	}

	// indirect modes (zp,x) and (zp),y

	if (token == '(') {
		scan_token();
		scan_value();
		if (pass == 2 && value > 255)
			scan_error("Operand not in zero page");
		switch (token) {
		case ',':
			scan_token();
			if (token != T_NAME)
				break;
			if (strcmp(tokenbuf, "x") != 0 && strcmp(tokenbuf, "X") != 0)
				break;
			scan_token();
			if (token != ')')
				break;
			scan_token();
			addrmode = AM_ZPXI;
			return;
		case ')':
			scan_token();
			if (token != ',')
				break;
			scan_token();
			if (token != T_NAME)
				break;
			if (strcmp(tokenbuf, "y") != 0 && strcmp(tokenbuf, "Y") != 0)
				break;
			scan_token();
			addrmode = AM_ZPIY;
			return;
		}
		scan_error("Expected indirect form (ZP,X) or (ZP),Y");
		return;
	}

	// absolute modes

	scan_value();

	if (token == ',') {
		scan_token();
		if (token == T_NAME) {
			if (strcmp(tokenbuf, "x") == 0 || strcmp(tokenbuf, "X") == 0) {
				scan_token();
				addrmode = AM_ABSX;
				return;
			}
			if (strcmp(tokenbuf, "y") == 0 || strcmp(tokenbuf, "Y") == 0) {
				scan_token();
				addrmode = AM_ABSY;
				return;
			}
		}
		scan_error("Expected index X or Y after comma");
	}

	addrmode = AM_ABS;
}

void
scan_expr(void)
{
	int t = token;

	if (t == '+' || t == '-' || t == '~' || t == '<' || t == '>')
		scan_token();

	scan_term();

	switch (t) {
	case '-':
		stack_neg();
		break;
	case '~':
		stack_cpl();
		break;
	case '>':
		stack_lo();
		break;
	case '<':
		stack_hi();
		break;
	default:
		break;
	}

	while (token == '+' || token == '-' || token == '&' || token == '|' || token == '^' || token == T_SHR || token == T_SHL) {
		t = token;
		scan_token();
		scan_term();
		switch (t) {
		case '+':
			stack_add();
			break;
		case '-':
			stack_sub();
			break;
		case '&':
			stack_and();
			break;
		case '|':
			stack_or();
			break;
		case '^':
			stack_xor();
			break;
		case T_SHR:
			stack_shr();
			break;
		case T_SHL:
			stack_shl();
			break;
		}
	}
}

void
scan_term(void)
{
	int t;
	scan_factor();
	while (token == '*' || token == '/' || token == '%') {
		t = token;
		scan_token();
		scan_factor();
		switch (t) {
		case '*':
			stack_mul();
			break;
		case '/':
			stack_div();
			break;
		case '%':
			stack_rem();
			break;
		}
	}
}

void
scan_factor(void)
{
	int value;
	char *s;
	struct sym *p;

	switch (token) {

	case '(':
		scan_token();
		scan_expr();
		if (token != ')')
			scan_error("Expected closing paren ')'");
		break;

	case '*':
	case '$':
		stack_push(curloc);
		break;

	case T_NAME:
		p = scan_lookup();
		if (p) {
			stack_push(p->value);
			if (p->where > where)
				where = p->where;
		} else {
			if (pass == 2)
				scan_error("Undefined symbol");
			where = UNDEF; // forward ref
			stack_push(0); // dummy value
		}
		break;

	case T_DECSTR:
		value = 0;
		s = tokenbuf;
		while (*s)
			value = 10 * value + *s++ - '0';
		stack_push(value);
		break;

	case T_HEXSTR:
		value = 0;
		s = tokenbuf;
		while (*s)
			if (*s <= '9')
				value = (value << 4) | (*s++ - '0');
			else
				value = (value << 4) | (tolower(*s++) - 'a' + 10);
		stack_push(value);
		break;

	case T_QUOSTR:
		stack_push(*tokenbuf);
		break;

	default:
		scan_error("Syntax error");
		break;
	}

	scan_token(); // get next token
}

void
scan_error(char *errmsg)
{
	char *s;
	printf("Line %d:\n", curlin);
	s = lineptr;
	while (*s && *s != '\n' && s != scanptr)
		putchar(*s++);
	putchar('?');
	while (*s && *s != '\n')
		putchar(*s++);
	putchar('\n');
	puts(errmsg);
	exit(1);
}

struct sym *
scan_lookup(void)
{
	int i, k;
	k = tolower(*tokenbuf) - 'a';
	for (i = 0; i < NSYM; i++) {
		if (stab[k][i].name == NULL)
			break; // not found
		if (strcmp(stab[k][i].name, tokenbuf) == 0)
			return &stab[k][i];
	}
	return NULL;
}

// scanptr points to start of line

void
scan_token(void)
{
	while (*scanptr == ' ' || *scanptr == '\t')
		scanptr++;

	if (*scanptr == ';')
		while (*scanptr && *scanptr != '\n')
			scanptr++;

	if (*scanptr == 0) {
		token = T_LF;
		return;
	}

	if (*scanptr == '\n') {
		token = T_LF;
		scanptr++;
		return;
	}

	tokenptr = scanptr;

	if (isalpha(*scanptr)) {
		do
			scanptr++;
		while (isalnum(*scanptr));
		tokenlen = scanptr - tokenptr;
		if (tokenlen > TOKENBUFLEN)
			scan_error("Too long");
		memcpy(tokenbuf, tokenptr, tokenlen);
		tokenbuf[tokenlen] = 0;
		if (lineptr - tokenptr == 0) {
			token = T_LABEL;
			if (*scanptr == ':')
				scanptr++; // ':' is optional
		} else
			token = T_NAME;
		return;
	}

	if (isdigit(*scanptr)) {
		while (isdigit(*scanptr))
			scanptr++;
		tokenlen = scanptr - tokenptr;
		if (tokenlen > TOKENBUFLEN)
			scan_error("Too long");
		memcpy(tokenbuf, tokenptr, tokenlen);
		tokenbuf[tokenlen] = 0;
		token = T_DECSTR;
		return;
	}

	if (*scanptr == '$') {
		scanptr++;
		if (isxdigit(*scanptr)) {
			while (isxdigit(*scanptr))
				scanptr++;
			tokenlen = scanptr - tokenptr - 1;
			if (tokenlen > TOKENBUFLEN)
				scan_error("Too long");
			memcpy(tokenbuf, tokenptr + 1, tokenlen);
			tokenbuf[tokenlen] = 0;
			token = T_HEXSTR;
		} else
			token = '$';
		return;
	}

	if (*scanptr == '"') {
		scanptr++;
		while (*scanptr && *scanptr != '"' && *scanptr != '\n')
			scanptr++;
		if (*scanptr != '"')
			scan_error("Runaway string");
		scanptr++;
		tokenlen = scanptr - tokenptr - 2;
		if (tokenlen > TOKENBUFLEN)
			scan_error("Too long");
		memcpy(tokenbuf, tokenptr + 1, tokenlen);
		tokenbuf[tokenlen] = 0;
		token = T_QUOSTR;
		return;
	}

	if (*scanptr == '\'') {
		scanptr++;
		if (*scanptr && scanptr[1] == '\'') {
			tokenbuf[0] = *scanptr;
			tokenbuf[1] = 0;
			tokenlen = 1;
			scanptr += 2;
			token = T_QUOSTR;
		} else
			scan_error("Syntax error");
		return;
	}

	if (*scanptr == '>' && scanptr[1] == '>') {
		token = T_SHR;
		scanptr += 2;
		return;
	}

	if (*scanptr == '<' && scanptr[1] == '<') {
		token = T_SHL;
		scanptr += 2;
		return;
	}

	token = *scanptr;
	scanptr++;
}

void
scan_branch(void)
{
	scan_value();
	value = value - (curloc + 2);
	if (pass == 2 && (value < -128 || value > 127))
		scan_error("Branch range error");
}

void
scan_value(void)
{
	where = 0;
	stackindex = 0;
	scan_expr();
	value = stack_pop();
}

void
scan_adc(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(ADC_IMM, 2);
		break;

	case AM_ZPXI:
		emit(ADC_ZPXI, 2);
		break;

	case AM_ZPIY:
		emit(ADC_ZPIY, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(ADC_ZP, 2);
		else
			emit(ADC_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(ADC_ZPX, 2);
		else
			emit(ADC_ABSX, 3);
		break;

	case AM_ABSY:
		emit(ADC_ABSY, 3);
		break;

	default:
		scan_error("Address mode error for ADC");
		break;
	}
}

void
scan_and(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(AND_IMM, 2);
		break;

	case AM_ZPXI:
		emit(AND_ZPXI, 2);
		break;

	case AM_ZPIY:
		emit(AND_ZPIY, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(AND_ZP, 2);
		else
			emit(AND_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(AND_ZPX, 2);
		else
			emit(AND_ABSX, 3);
		break;

	case AM_ABSY:
		emit(AND_ABSY, 3);
		break;

	default:
		scan_error("Address mode error for AND");
		break;
	}
}

void
scan_asl(void)
{
	scan_token();

	if (token == T_NAME && (strcmp(tokenbuf, "a") == 0 || strcmp(tokenbuf, "A") == 0)) {
		scan_token();
		emit(ASL_REGA, 1);
		return;
	}

	scan_addr();

	switch (addrmode) {

	case AM_ABS:
		if (ZPADDR)
			emit(ASL_ZP, 2);
		else
			emit(ASL_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(ASL_ZPX, 2);
		else
			emit(ASL_ABSX, 3);
		break;

	default:
		scan_error("Address mode error for ASL");
		break;
	}
}

void
scan_bcc(void)
{
	scan_token();
	scan_branch();
	emit(OP_BCC, 2);
}

void
scan_bcs(void)
{
	scan_token();
	scan_branch();
	emit(OP_BCS, 2);
}

void
scan_beq(void)
{
	scan_token();
	scan_branch();
	emit(OP_BEQ, 2);
}

void
scan_bit(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_ABS:
		if (ZPADDR)
			emit(BIT_ZP, 2);
		else
			emit(BIT_ABS, 3);
		break;

	default:
		scan_error("Address mode error for BIT");
		break;
	}
}

void
scan_bmi(void)
{
	scan_token();
	scan_branch();
	emit(OP_BMI, 2);
}

void
scan_bne(void)
{
	scan_token();
	scan_branch();
	emit(OP_BNE, 2);
}

void
scan_bpl(void)
{
	scan_token();
	scan_branch();
	emit(OP_BPL, 2);
}

void
scan_brk(void)
{
	scan_token();
	emit(OP_BRK, 1);
}

void
scan_bvc(void)
{
	scan_token();
	scan_branch();
	emit(OP_BVC, 2);
}

void
scan_bvs(void)
{
	scan_token();
	scan_branch();
	emit(OP_BVS, 2);
}

void
scan_clc(void)
{
	scan_token();
	emit(OP_CLC, 1);
}

void
scan_cld(void)
{
	scan_token();
	emit(OP_CLD, 1);
}

void
scan_cli(void)
{
	scan_token();
	emit(OP_CLI, 1);
}

void
scan_clv(void)
{
	scan_token();
	emit(OP_CLV, 1);
}

void
scan_cmp(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(CMP_IMM, 2);
		break;

	case AM_ZPXI:
		emit(CMP_ZPXI, 2);
		break;

	case AM_ZPIY:
		emit(CMP_ZPIY, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(CMP_ZP, 2);
		else
			emit(CMP_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(CMP_ZPX, 2);
		else
			emit(CMP_ABSX, 3);
		break;

	case AM_ABSY:
		emit(CMP_ABSY, 3);
		break;

	default:
		scan_error("Address mode error for CMP");
		break;
	}
}

void
scan_cpx(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(CPX_IMM, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(CPX_ZP, 2);
		else
			emit(CPX_ABS, 3);
		break;

	default:
		scan_error("Address mode error for CPX");
		break;
	}
}

void
scan_cpy(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(CPY_IMM, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(CPY_ZP, 2);
		else
			emit(CPY_ABS, 3);
		break;

	default:
		scan_error("Address mode error for CPY");
		break;
	}
}

void
scan_dec(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_ABS:
		if (ZPADDR)
			emit(DEC_ZP, 2);
		else
			emit(DEC_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(DEC_ZPX, 2);
		else
			emit(DEC_ABSX, 3);
		break;

	default:
		scan_error("Address mode error for DEC");
		break;
	}
}

void
scan_dex(void)
{
	scan_token();
	emit(OP_DEX, 1);
}

void
scan_dey(void)
{
	scan_token();
	emit(OP_DEY, 1);
}

void
scan_eor(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(EOR_IMM, 2);
		break;

	case AM_ZPXI:
		emit(EOR_ZPXI, 2);
		break;

	case AM_ZPIY:
		emit(EOR_ZPIY, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(EOR_ZP, 2);
		else
			emit(EOR_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(EOR_ZPX, 2);
		else
			emit(EOR_ABSX, 3);
		break;

	case AM_ABSY:
		emit(EOR_ABSY, 3);
		break;

	default:
		scan_error("Address mode error for EOR");
		break;
	}
}

void
scan_inc(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_ABS:
		if (ZPADDR)
			emit(INC_ZP, 2);
		else
			emit(INC_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(INC_ZPX, 2);
		else
			emit(INC_ABSX, 3);
		break;

	default:
		scan_error("Address mode error for INC");
		break;
	}
}

void
scan_inx(void)
{
	scan_token();
	emit(OP_INX, 1);
}

void
scan_iny(void)
{
	scan_token();
	emit(OP_INY, 1);
}

void
scan_jmp(void)
{
	scan_token();
	if (token == '(') {
		scan_token();
		scan_value();
		if (token != ')')
			scan_error("Expected closing paren ')'");
		scan_token();
		emit(OP_JMPI, 3);
	} else {
		scan_value();
		emit(OP_JMP, 3);
	}
}

void
scan_jsr(void)
{
	scan_token();
	scan_value();
	emit(OP_JSR, 3);
}

void
scan_lda(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(LDA_IMM, 2);
		break;

	case AM_ZPXI:
		emit(LDA_ZPXI, 2);
		break;

	case AM_ZPIY:
		emit(LDA_ZPIY, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(LDA_ZP, 2);
		else
			emit(LDA_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(LDA_ZPX, 2);
		else
			emit(LDA_ABSX, 3);
		break;

	case AM_ABSY:
		emit(LDA_ABSY, 3);
		break;

	default:
		scan_error("Address mode error for LDA");
		break;
	}
}

void
scan_ldx(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(LDX_IMM, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(LDX_ZP, 2);
		else
			emit(LDX_ABS, 3);
		break;

	case AM_ABSX:
	case AM_ABSY:
		if (ZPADDR)
			emit(LDX_ZPX, 2);
		else
			emit(LDX_ABSX, 3);
		break;

	default:
		scan_error("Address mode error for LDX");
		break;
	}
}

void
scan_ldy(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(LDY_IMM, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(LDY_ZP, 2);
		else
			emit(LDY_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(LDY_ZPX, 2);
		else
			emit(LDY_ABSX, 3);
		break;

	default:
		scan_error("Address mode error for LDY");
		break;
	}
}

void
scan_lsr(void)
{
	scan_token();

	if (token == T_NAME && (strcmp(tokenbuf, "a") == 0 || strcmp(tokenbuf, "A") == 0)) {
		scan_token();
		emit(LSR_REGA, 1);
		return;
	}

	scan_addr();

	switch (addrmode) {

	case AM_ABS:
		if (ZPADDR)
			emit(LSR_ZP, 2);
		else
			emit(LSR_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(LSR_ZPX, 2);
		else
			emit(LSR_ABSX, 3);
		break;

	default:
		scan_error("Address mode error for LSR");
		break;
	}
}

void
scan_nop(void)
{
	scan_token();
	emit(OP_NOP, 1);
}

void
scan_ora(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(ORA_IMM, 2);
		break;

	case AM_ZPXI:
		emit(ORA_ZPXI, 2);
		break;

	case AM_ZPIY:
		emit(ORA_ZPIY, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(ORA_ZP, 2);
		else
			emit(ORA_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(ORA_ZPX, 2);
		else
			emit(ORA_ABSX, 3);
		break;

	case AM_ABSY:
		emit(ORA_ABSY, 3);
		break;

	default:
		scan_error("Address mode error for ORA");
		break;
	}
}

void
scan_pha(void)
{
	scan_token();
	emit(OP_PHA, 1);
}

void
scan_php(void)
{
	scan_token();
	emit(OP_PHP, 1);
}

void
scan_pla(void)
{
	scan_token();
	emit(OP_PLA, 1);
}

void
scan_plp(void)
{
	scan_token();
	emit(OP_PLP, 1);
}

void
scan_rol(void)
{
	scan_token();

	if (token == T_NAME && (strcmp(tokenbuf, "a") == 0 || strcmp(tokenbuf, "A") == 0)) {
		scan_token();
		emit(ROL_REGA, 1);
		return;
	}

	scan_addr();

	switch (addrmode) {

	case AM_ABS:
		if (ZPADDR)
			emit(ROL_ZP, 2);
		else
			emit(ROL_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(ROL_ZPX, 2);
		else
			emit(ROL_ABSX, 3);
		break;

	default:
		scan_error("Address mode error for ROL");
		break;
	}
}

void
scan_ror(void)
{
	scan_token();

	if (token == T_NAME && (strcmp(tokenbuf, "a") == 0 || strcmp(tokenbuf, "A") == 0)) {
		scan_token();
		emit(ROR_REGA, 1);
		return;
	}

	scan_addr();

	switch (addrmode) {

	case AM_ABS:
		if (ZPADDR)
			emit(ROR_ZP, 2);
		else
			emit(ROR_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(ROR_ZPX, 2);
		else
			emit(ROR_ABSX, 3);
		break;

	default:
		scan_error("Address mode error for ROR");
		break;
	}
}

void
scan_rti(void)
{
	scan_token();
	emit(OP_RTI, 1);
}

void
scan_rts(void)
{
	scan_token();
	emit(OP_RTS, 1);
}

void
scan_sbc(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_IMM:
		emit(SBC_IMM, 2);
		break;

	case AM_ZPXI:
		emit(SBC_ZPXI, 2);
		break;

	case AM_ZPIY:
		emit(SBC_ZPIY, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(SBC_ZP, 2);
		else
			emit(SBC_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(SBC_ZPX, 2);
		else
			emit(SBC_ABSX, 3);
		break;

	case AM_ABSY:
		emit(SBC_ABSY, 3);
		break;

	default:
		scan_error("Address mode error for SBC");
		break;
	}
}

void
scan_sec(void)
{
	scan_token();
	emit(OP_SEC, 1);
}

void
scan_sed(void)
{
	scan_token();
	emit(OP_SED, 1);
}

void
scan_sei(void)
{
	scan_token();
	emit(OP_SEI, 1);
}

void
scan_sta(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_ZPXI:
		emit(STA_ZPXI, 2);
		break;

	case AM_ZPIY:
		emit(STA_ZPIY, 2);
		break;

	case AM_ABS:
		if (ZPADDR)
			emit(STA_ZP, 2);
		else
			emit(STA_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(STA_ZPX, 2);
		else
			emit(STA_ABSX, 3);
		break;

	case AM_ABSY:
		emit(STA_ABSY, 3);
		break;

	default:
		scan_error("Address mode error for STA");
		break;
	}
}

void
scan_stx(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_ABS:
		if (ZPADDR)
			emit(STX_ZP, 2);
		else
			emit(STX_ABS, 3);
		break;

	case AM_ABSX:
	case AM_ABSY:
		if (ZPADDR)
			emit(STX_ZPX, 2);
		else
			scan_error("Zero page address required for STX with X");
		break;

	default:
		scan_error("Address mode error for STX");
		break;
	}
}

void
scan_sty(void)
{
	scan_token();
	scan_addr();

	switch (addrmode) {

	case AM_ABS:
		if (ZPADDR)
			emit(STY_ZP, 2);
		else
			emit(STY_ABS, 3);
		break;

	case AM_ABSX:
		if (ZPADDR)
			emit(STY_ZPX, 2);
		else
			scan_error("STY with X requires zero page address");
		break;

	default:
		scan_error("Address mode error for STY");
		break;
	}
}

void
scan_tax(void)
{
	scan_token();
	emit(OP_TAX, 1);
}

void
scan_tay(void)
{
	scan_token();
	emit(OP_TAY, 1);
}

void
scan_tsx(void)
{
	scan_token();
	emit(OP_TSX, 1);
}

void
scan_txa(void)
{
	scan_token();
	emit(OP_TXA, 1);
}

void
scan_txs(void)
{
	scan_token();
	emit(OP_TXS, 1);
}

void
scan_tya(void)
{
	scan_token();
	emit(OP_TYA, 1);
}
void
stack_push(int value)
{
	if (stackindex >= STACKSIZE)
		scan_error("stack overflow");
	stack[stackindex++] = value;
}

int
stack_pop(void)
{
	if (stackindex < 1)
		scan_error("stack underflow");
	return stack[--stackindex];
}

void
stack_add(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	stack[stackindex - 1] += stack[stackindex];
}

void
stack_sub(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	stack[stackindex - 1] -= stack[stackindex];
}

void
stack_neg(void)
{
	if (stackindex < 1)
		scan_error("stack underflow");
	stack[stackindex - 1] = -stack[stackindex - 1];
}

void
stack_mul(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	stack[stackindex - 1] *= stack[stackindex];
}

void
stack_div(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	if (where == UNDEF)
		return; // forward ref, defer until pass 2
	if (stack[stackindex] == 0)
		scan_error("divide by zero");
	stack[stackindex - 1] /= stack[stackindex];
}

void
stack_rem(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	if (where == UNDEF)
		return; // forward ref, defer until pass 2
	if (stack[stackindex] == 0)
		scan_error("divide by zero");
	stack[stackindex - 1] %= stack[stackindex];
}

void
stack_and(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	stack[stackindex - 1] &= stack[stackindex];
}

void
stack_or(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	stack[stackindex - 1] |= stack[stackindex];
}

void
stack_xor(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	stack[stackindex - 1] ^= stack[stackindex];
}

void
stack_cpl(void)
{
	if (stackindex < 1)
		scan_error("stack underflow");
	stack[stackindex - 1] = ~stack[stackindex - 1];
}

void
stack_shr(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	stack[stackindex - 1] >>= stack[stackindex];
}

void
stack_shl(void)
{
	if (stackindex < 2)
		scan_error("stack underflow");
	stackindex--;
	stack[stackindex - 1] <<= stack[stackindex];
}

void
stack_lo(void)
{
	if (stackindex < 1)
		scan_error("stack underflow");
	stack[stackindex - 1] = stack[stackindex - 1] & 0xff;
}


void
stack_hi(void)
{
	if (stackindex < 1)
		scan_error("stack underflow");
	stack[stackindex - 1] = stack[stackindex - 1] >> 8;
}
