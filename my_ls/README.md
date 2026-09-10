# my_ls

An implementation of a subset of GNU `ls` written from scratch in C, without looking at the GNU `ls` source code.

The project is intended to closely reproduce GNU `ls` behaviour while practicing Unix system programming, file metadata handling, terminal output, and low-level C.

## Current functionality

- Default directory listings
- Multiple file and directory operands
- Invalid-path handling
- Locale-aware sorting with `strcoll()`
- Terminal-width-aware column layout
- Redirected-output detection
- `-a` / `--all`
- `-l`
- Combined `-la` / `-al`
- `--help`
- `--version`

Long listings currently handle:

- permissions and file types
- hard-link counts
- user/group names
- aligned file sizes
- character/block device major and minor numbers
- timestamps
- symbolic link targets
- directory block totals

Supported file types include regular files, directories, symbolic links, FIFOs, Unix sockets, character devices, and block devices.

## Build

Build `my_ls` using the included Makefile:
`make my_ls`

## What I learned

**Terminal layout**: GNU `ls` fills entries down columns and sizes each column based on its longest filename. I had to try different row counts, calculate column widths, and pick the densest layout that fit the terminal (using ioctl and isatty()). I also had to handle narrow viewports, TTY vs redirected output, and test widths from 1–400 columns against real ls. A number of challenges arose in calculating the width of the column to match GNU `ls`.

**Filesystem metadata**: Implementing `-l` required working directly with `stat()`/`lstat()`, `mode_t` permission bits, link counts, UID/GID lookups, timestamps, symlinks, and Unix file types. Character and block devices also meant displaying major/minor numbers instead of normal file sizes.

Matching GNU behavior: Cloning `ls` behaviour exposed edge cases I wouldn't have considered otherwise: file vs directory operands, invalid paths, hidden files, locale sorting with `strcoll()`, and flag combinations. Diff testing against `ls` was how I determined if my clone matched `ls` behaviour.

## Testing

To run test suite:
`make ls_tests` 
This does not include terminal-width tests.

Run the complete test suite, including terminal-width testing with:
`make ls_all_tests`

Tests compare `my_ls` directly against GNU `ls`.

Coverage includes:

- default, `-a`, `-l`, and `-la`
- multiple and mixed operands
- invalid paths
- hard links and symbolic links
- FIFOs, sockets, character devices, and block devices
- timestamp edge cases
- zero-value formatting
- empty directories

Terminal layout is compared against GNU `ls` across terminal widths from 1 to 400 columns.

The test build is compiled with:

`-Wall -Wextra -std=c99 -fsanitize=address,undefined`

## Scope

This project implements a subset of GNU `ls` rather than the full GNU `ls` option set. The scope includes default listings, `-a`/`--all`, `-l`, combined `-la`/`-al`, multiple operands, terminal-aware column formatting, and the file types and long-listing behaviour described above.

Additional GNU `ls` options are not in the planned scope as this was a learning tool and I have moved on to other projects.

## References

- W. Richard Stevens and Stephen A. Rago, `Advanced Programming in the UNIX Environment`, 3rd ed. — used as a reference for Unix/POSIX system interfaces and filesystem behaviour.
- Linux/POSIX manual pages — used as references for system calls and library functions.
- GNU ls — used as the behavioural reference for differential testing. The GNU ls source code was not consulted during implementation.



