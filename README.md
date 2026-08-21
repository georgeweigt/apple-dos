Assembly source file `apple-dos.s` is a union of files from the `Apple DOS 3.3C Source Code` archive.
Object code matches byte-for-byte the contents of `Apple DOS 3.3 January 1983.dsk`.

Build assembler `asm` and generate a listing file

```
make
./asm apple-dos.s > listing.txt
```

Compare assembled code with Apple DOS 3.3 disk

```
make verify
```

Create `patchfile` of differences between the original source files and `apple-dos.s`

```
make patchfile
```
