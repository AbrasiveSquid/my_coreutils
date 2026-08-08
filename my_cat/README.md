# my_cat

My implementation of the Unix 'cat' coreutil written in C.

This is a part of my [my_coreutils](...) project, where I am rewriting a number of Unix coreutils to better understand
C, Unix, and systems programming.

## Features

- `-A` - equivalent to -vET, alternatively can use `--show-all`
- `-b` - number nonempty output lines, overrides -n
- `-e`- equivalent to -vE
- `-E` - display `$` at end of each line, alternatively can use `--show-ends`
- `-n` - number all output lines, alternatively can use `--number`
- `-s` - suppress repeated empty output lines, alternatively can use `--squeeze-blank`
- `-t` - equivalent to -vT
- `-T` - display TAB characters as ^I, alternatively can use `--show-tabs`
- `-u` - ignored, included for backwards compatibility
- `-v` - use ^ and M- notation, except for LFD and TAB
- `--help` - brings up the help menu
- `--version` - shows version details

- Supports multiple input files
- Reading from `stdin` with `-`
- Supports multiple `-` arguments
- Supports combining multiple flags, for example `-ns` or `-n` `-s` are equivalent
- Supports flags anywhere among the command line arguments:
  - `./my_cat file.txt -n`
  - `./my_cat -n file.txt`
  - `./my_cat file1.txt -n file2.txt`


## Build
```bash
make
```

## Usage
```bash
./my_cat file.txt
./my_cat -n -
./my_cat -ns file.txt
./my_cat file1.txt file2.txt -b
./my_cat
```

## Testing
```bash
make cat_tests
```
Tests cover all flags, a combination of flags, multiple files, stdin, empty lines, files without trailing newline and other edge cases. Tests are compared against cat output to show my program's output is the same as the system `cat` program.


## What I learned
- C command line arguments
- FILE and standard I/O
- File streams, stream state, error handling
- Command line argument parsing
- Handling stdin and clearing EOF
- Automated testing

- One of the more difficult problems was handling multiple `-` arguments, which allows the program to take input from `stdin` multiple times. I learned that when `stdin` reaches `EOF`, that `stdin` retains its `EOF` indicator. Subsequent attempts to take input from `stdin` will immediately return as the print to `stdout` loop is checking for `EOF`. I discovered I can check if a file stream has reached `EOF` with `feof()` and can clear it with `clearerr()`, before reading from the stream.
