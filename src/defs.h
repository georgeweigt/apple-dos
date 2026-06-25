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
