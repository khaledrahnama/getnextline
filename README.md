*This project has been created as part of the 42 curriculum
by krahnama.*

# get_next_line

## Description

The `get_next_line`42 Project purpose is tp return one new line in each call while remembering lines which were returned in previous calls. The bonus part of the project is performing the same on more than one file. 


## Instructions

### Compilation

 `BUFFER_SIZE` supplied at compile time via `-D`:

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

- The C `read(2)`, `malloc(3)`, and `free(3)` man pages. 
- The 42 subject PDF for `get_next_line`.
- YouTube
- https://pythontutor.com/

**Use of AI:** For debugging and understanding the algorithm

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
  reason about, but in many cases has many disadvantages

**Reading the whole file into memory at once** would be simpler to
  reason about, but in many cases has many disadvantages:

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
  - **It's wasteful when only part of the file is needed.** 
  - **It defeats the purpose of the exercise.** 

- **Re-reading from the start of the file on every call** (seeking
  back with `lseek`) would avoid needing static state, but is far
  less efficient (O(n²) total reads for a file of n lines) and doesn't
  generalize to non-seekable file descriptors like pipes or sockets,
  which the bonus part is expected to support.
- **The persistent stash**

### Why an array of stashes for the bonus

The bonus requirement — correct behavior when interleaving calls on
different fds — rules out a single global stash, since reading fd A
would silently corrupt or discard fd B's in-progress buffer. Indexing
the stash by fd (`static char *stash[MAX_FD]`) isolates each fd's state
completely while reusing the exact same fill/extract/keep logic, which
keeps the bonus implementation a strict superset of the mandatory one
rather than a rewrite.

### Complexity

- **Time:** O(n) overall for a file of n bytes, across all calls combined.
- **Space:** O(BUFFER_SIZE) for the read buffer plus O(length of the
  longest line) for the stash , depends on the file length.