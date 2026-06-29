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
		return; // pass 1 forward defn
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
		return; // pass 1 forward defn
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
