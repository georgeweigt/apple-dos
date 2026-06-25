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
