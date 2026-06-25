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
