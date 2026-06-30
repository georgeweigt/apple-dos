Build assembler `asm` and generate a listing file (`%` is shell prompt)

```
% make
% ./asm apple-dos.s > listing.txt
```

Compare assembled code with Apple DOS 3.3 disk

```
% make verify
./asm apple-dos.s "Apple DOS 3.3 January 1983.dsk"
8781 bytes compared
0 compare errors
```

`DOS33C.pretty` is the top level file in `Apple DOS 3.3C Source Code`

To build `apple-dos-orig.s` and `patchfile`

```
% make all
```
