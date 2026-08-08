# my_cat

My implementation of the Unix 'cat' coreutil written in C.

This is a part of my [my_coreutils](...) project, where I am rewriting a number of Unix coretils to better understand
C, Unix, and systems program.

## Features

- `-A` - equivalent to -vET, alternatively can use `--show-all`
- `-b` - number nonempty outling lines, overrides -n
- `-e`- equivalent to -vE
- `-E` - display$ at end of each line, alternatively can use `--show-ends`
- `-n` - number all output lines, alternatively can use `--number`
- `-s` - suppress repeated empty outline lines, alternatively can use `--squeeze-blank` 
- `-t` - equivalent to -vT
- `-T` - display TAB characters as ^I, alternatively can use `--show-tabs`
- `-u` - ignored, included for backwards compatibility
- `-v` - use ^ and M- notation, except for LFD and TAB
- `--help` - brings up the help menu
- `--version` - shows version details

- Supports multiple input files
- Reading from `stdin` with `-`
- Multiple `-` arguments
- Combining multiple flags, for example `-ns` or `-n` `-s` are equivalent 


## Build
```bash
```bash
make
```
```
```

## Usage
```bash
```bash 
./my_cat file.txt
./my_cat -n -
./my_cat -ns file.txt
./my_cat file1.txt file2.txt -b
```
```






```

```
```
```
```
```
```
```
```




