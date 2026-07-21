*This project has been created as part of the 42 curriculum
by khaledrahnama.*

# get_next_line

## Description

`get_next_line` is a 42 School project whose goal is to write a
function, `get_next_line(fd)`, that returns exactly one line at a time
from a file descriptor, on every successive call — as if the file were
being read through a cursor that remembers where it left off.

The challenge is that the only tool available for reading is `read()`,
which has no concept of "a line." It just returns raw bytes in chunks
of a fixed size (`BUFFER_SIZE`), so a single call to `read()` might
return only part of a line, several lines at once, or no complete line
at all. `get_next_line` buffers, cuts, and stitches this raw data
itself, using a `static` variable to persist state across calls, and
hands back one clean line (including its trailing `\n`, if present)
each time it's called, until the file is exhausted (`NULL`).

The bonus part extends this to work correctly when the function is
called on **several file descriptors interleaved** — for example,
alternating calls between two different open files — without one fd's
read progress interfering with another's.

## Instructions

### Compilation

`BUFFER_SIZE` is supplied at compile time via `-D`:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   your_main.c get_next_line.c get_next_line_utils.c \
   -o your_program
```

For the bonus part:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   your_main.c get_next_line_bonus.c get_next_line_utils_bonus.c \
   -o your_program
```

If `BUFFER_SIZE` is not defined at compile time, it defaults to `42`.

> Note: the bonus files (`get_next_line_bonus.c`,
> `get_next_line_bonus.h`, `get_next_line_utils_bonus.c`) must be
> present in the same directory for the second command to work.

### Installation

No external dependencies are required beyond a standard C toolchain
(`gcc` or `clang`) and the C standard library (`read`, `malloc`,
`free`). Clone the repository and compile as shown above:

```bash
git clone <repo_url>
cd get_next_line
```

### Usage

Include the header and call the function on any valid, open file
descriptor:

```c
#include "get_next_line.h"

int fd = open("some_file.txt", O_RDONLY);
char *line;

while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
close(fd);
```

## Resources

- The C `read(2)`, `malloc(3)`, and `free(3)` man pages — the only
  functions this project allows for I/O and memory management.
- The 42 subject PDF for `get_next_line`, which specifies the exact
  function signature, allowed functions, and norm constraints.
- General notes on buffered I/O and the difference between
  byte-oriented and line-oriented reading, used to reason about why a
  persistent buffer (the "stash") is necessary between calls.

**Use of AI:** An AI assistant (Claude) was used for the following,
specific parts of this project:
- Reviewing the mandatory `get_next_line.c` / `get_next_line_utils.c`
  implementation for logic bugs, memory leaks, and 42 Norm violations,
  and explaining *why* each flagged issue was or wasn't a real problem
  (traced by hand through multi-line files, no-trailing-newline files,
  empty files, and `read()` failure cases).
- Drafting the bonus implementation (`get_next_line_bonus.c`,
  `get_next_line_bonus.h`, `get_next_line_utils_bonus.c`), matching the
  style and helper-function names of the existing mandatory part.
- Explaining how to set up and run memory-leak checks (AddressSanitizer,
  macOS `leaks`, and `valgrind` via a Linux Docker container), and how
  to read their output.
- Debugging issues in `main.c` (a wrong `argv` index, loop-ordering bugs
  around calling `get_next_line` before checking its result, a
  redundant/leaked `open()` call, and declarations placed after
  statements).
- Helping draft and correct this README.

All AI-suggested code and fixes were reviewed and tested (including
against `valgrind`) before being included, and are understood well
enough to be explained and modified independently.

## Algorithm: Selection and Justification

### The chosen approach: a persistent "stash" buffer

The implementation keeps one `static char *stash` (or, for the bonus,
one stash per file descriptor) that accumulates raw bytes read from
the file. Each call to `get_next_line` performs three steps:

1. **Fill** — repeatedly call `read()` into a fixed-size local buffer
   and append it to the stash, until the stash contains a `\n` or
   `read()` signals EOF/error.
2. **Extract** — copy out everything from the start of the stash up to
   and including the first `\n` (or the entire stash, if none is
   found and the file has ended) as the line to return.
3. **Keep** — copy out everything *after* that `\n` and store it back
   in the stash for the next call, discarding what was already
   returned.

### Why this approach, over the alternatives

- **Reading the whole file into memory at once** would be simpler to
  reason about, but has several disadvantages:
  - **Memory cost scales with file size.** A file of several gigabytes
    would require allocating several gigabytes of memory up front,
    even if the caller only ever wants the first few lines — whereas
    the stash approach only ever holds a small, bounded amount of data
    (roughly `BUFFER_SIZE` plus the length of the longest single line).
  - **It doesn't work on non-seekable or infinite streams.** Pipes,
    sockets, and other special file descriptors don't have a
    well-defined "whole file" to read in one shot — some may never
    reach EOF, or may only make new data available gradually. The
    bonus part specifically exercises this kind of fd, which a
    read-everything-at-once strategy can't support.
  - **It's wasteful when only part of the file is needed.** If a
    caller stops reading after the first few lines, a whole-file read
    has already paid the full cost of reading and buffering the entire
    file, while the stash approach only ever reads as far as it's
    asked to.
  - **It defeats the purpose of the exercise.** The project is
    specifically designed around the constraint of reading through a
    fixed-size buffer via repeated `read()` calls; loading everything
    at once sidesteps the core problem the subject is testing and is
    disallowed by the subject for exactly that reason.

- **Re-reading from the start of the file on every call** (seeking
  back with `lseek`) would avoid needing static state, but is far
  less efficient (O(n²) total reads for a file of n lines) and doesn't
  generalize to non-seekable file descriptors like pipes or sockets,
  which the bonus part is expected to support.

- **The persistent stash** avoids both problems above: it reads each
  byte of the file exactly once overall (amortized across calls), uses
  bounded memory regardless of file size, and works identically on
  regular files, pipes, and other fd types, since it only ever reads
  forward and never seeks.

### Why an array of stashes for the bonus

The bonus requirement — correct behavior when interleaving calls on
different fds — rules out a single global stash, since reading fd A
would silently corrupt or discard fd B's in-progress buffer. Indexing
the stash by fd (`static char *stash[MAX_FD]`) isolates each fd's state
completely while reusing the exact same fill/extract/keep logic, which
keeps the bonus implementation a strict superset of the mandatory one
rather than a rewrite.

### Complexity

- **Time:** each byte of the file is read once and copied a small,
  constant number of times — O(n) overall for a file of n bytes,
  across all calls combined.
- **Space:** O(BUFFER_SIZE) for the read buffer plus O(length of the
  longest line) for the stash at any given moment — never proportional
  to the total file size.
