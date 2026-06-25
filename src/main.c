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
