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
emit_word(int word)
{
	emit_byte(word);
	emit_byte(word >> 8);
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
		if (curloc < 0x3600)
			index = curloc - 0x1b00 + 2560; // start at track 0, sector 10
		else if (curloc < 0x4000)
			index = curloc - 0x3600; // start at track 0, sector 0
		else {
			printf("address error 0x%04x\n", curloc);
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
