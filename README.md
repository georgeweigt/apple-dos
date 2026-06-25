Build `asm` and generate a listing file (`%` is shell prompt)

```
% make
% ./asm apple-dos.s > listing.txt
```

Compare assembled code with Apple DOS 3.3 disk

```
% ./asm apple-dos.s "Apple DOS 3.3 January 1983.dsk"
8781 bytes compared
0 compare errors
```
