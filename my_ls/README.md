# my_ls

In progress implementation of GNU `ls` written from scratch (without looking at `ls` source code) in C.

Currently reproduces the default no-option (flag) behaviour. Currently working on implementing flags.

## Current functionality

- Lists directory contents using `opendir()` / `readdir()`
- Dynamically stores directory entires with `realloc()`
- Sorts filenames using `strcoll()`
- Detects redirected output with `isatty()`
- Reads terminal width using `ioctl(TIOCGWINSZ)`
- Calculates a column-major layout based on terminal width and filename lengths
- Falls back to single-column output when terminal width is too small or output redirected

## Testing

Tests compare `my_ls` directly against GNU `ls`.

The layout tests run both programs inside pseudo-terminals and compare their output across terminal widths from 1-400 columns.

The project is compiled with:

`-Wall -Wextra -std=c99 -fsanitize=address,undefined`


## Status

Default no-option (no flags) behaviour implemented. Currently working on parsing and interpreting command line flags.
