# my_ls

An in-progress implementation of GNU `ls` written from scratch in C, without looking at the GNU `ls` source code.

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

## Testing

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

The project is compiled with:

`-Wall -Wextra -std=c99 -fsanitize=address,undefined`

## Status

Complete:

- Default listing behaviour
- `-a` / `--all`
- `-l`
- Combined `-la` / `-al`

### Planned
- `--help`
- `--version`
- `-h`
- `-r`
- `-t`
- `-S`
- `-1`

