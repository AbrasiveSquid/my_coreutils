# Reimplementing coreutils in C



I am rewriting various [GNU Coreutils](https://www.gnu.org/software/coreutils/) utilities from scratch in C, only
looking at their help menu and output as a way to learn more about C and systems programming.


My goal is to not create a replacement or improvement on the existing coreutils, but to better understand
how these programs interface with the OS.

## coreutils

| program | description                  | status         |   
|---------|------------------------------|----------------|
| [`my_cat`](my_cat) | concatenate files to stdout | **completed** | <br> 
| [`my_ls`](my_ls) | list information about files | *in progress* |<br>


## Highlights
- Written in C99 using POSIX/Unix APIs
- Behaviour tested against the system GNU utilities (with diff)
- Built with AddressSanitizer and UndefiedHeaviorSanitizer
- Dynamic memory allocation and filesystem APIs
- Terminal detection and terminal-width-aware output
- Locale-aware filename sorting

[`my_cat`](my_cat) implements the GNU `cat` options (flags), including stdin handling, multiple files, how options interact with each other, and non-printing characters.

[`my_ls`](my_ls) currently reproduces the default no-option behaviour (no flags), including TTY detection, redirected
output, dynamic column layout, and testing across various terminal widths against GNU `ls` behaviour.

Each utility's subdirectory for implementation details, build instructions, and tests.
