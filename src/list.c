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
