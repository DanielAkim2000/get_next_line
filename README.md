*This project has been created as part of the 42 curriculum by demane.*

# get_next_line

## Description

**get_next_line** is a 42 project whose goal is to implement a C function that reads and returns one line at a time from a file descriptor.

Reading from a file descriptor with `read()` alone is inconvenient when you need line-oriented input: you must manage a buffer, detect `\n`, keep leftover data between calls, and handle edge cases (EOF without a final newline, errors, etc.). This project solves that by providing a reusable helper:

```c
char *get_next_line(int fd);
```

Repeated calls to `get_next_line()` let you traverse a file (or standard input) line by line. Each returned line includes the terminating `\n` character, except when the last line of the file does not end with one. When there is nothing left to read or an error occurs, the function returns `NULL`.

The project also introduces a key C concept: **static variables**, used here to preserve unread data between successive calls without relying on the caller to manage that state.

### Project overview

| Part | Content |
|------|---------|
| Mandatory | `get_next_line` for a single file descriptor (file or stdin) |
| Bonus | Same API with **one static variable** and support for **multiple file descriptors** at the same time |

---

## The library

Although there is no archive (`.a`) to link, the delivered files form a small self-contained reading library around `get_next_line`.

### Public API

| Function | Prototype | Role |
|----------|-----------|------|
| `get_next_line` | `char *get_next_line(int fd);` | Reads and returns the next line from `fd` |

**Parameters**

- `fd` — file descriptor to read from (opened file, `STDIN_FILENO`, etc.)

**Return value**

- A newly allocated string containing the line (including `\n` when present)
- `NULL` if there is nothing left to read, or if an error occurred

**Allowed external functions:** `read`, `malloc`, `free`

**Important:** the caller must `free()` the returned string after use.

### Internal design

The implementation is split into:

| File | Role |
|------|------|
| `get_next_line.c` / `get_next_line_bonus.c` | Core logic: read loop, line extraction, FD node management |
| `get_next_line_utils.c` / `get_next_line_utils_bonus.c` | Helpers (length, join, newline search, remainder handling, cleanup) |
| `get_next_line.h` / `get_next_line_bonus.h` | Prototypes, `BUFFER_SIZE`, and the `t_line` structure |

Reading is driven by a compile-time **`BUFFER_SIZE`**: each `read()` call fills a buffer of that size. Data is accumulated until a `\n` is found (or EOF is reached). Leftover bytes after the newline are kept for the next call via a static structure.

The bonus (and the linked-list approach used here) stores per-FD state in a list of `t_line` nodes:

```c
typedef struct s_line
{
	char			*line;  /* leftover / accumulated content */
	int				fd;     /* associated file descriptor */
	struct s_line	*next;
}	t_line;
```

A single static `t_line` anchors the list, so several descriptors can be read in an interleaved fashion without mixing their buffers.

### Behaviour summary

- Works with regular files and standard input
- Respects `BUFFER_SIZE` (including extreme values such as `1` or very large sizes)
- Returns `NULL` on invalid `fd`, invalid `BUFFER_SIZE`, read/allocation errors, or EOF with no remaining content
- Frees unused nodes / buffers when a descriptor is finished or fails

---

## Instructions

### Requirements

- A C compiler (`cc`)
- A Unix-like environment (`read`, file descriptors)

There is no Makefile in this project: you compile the sources directly (as specified by the subject).

### Compilation

Define `BUFFER_SIZE` at compile time with `-D` (any positive value; peers and Moulinette will change it):

```bash
# Mandatory
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
	get_next_line.c get_next_line_utils.c your_main.c -o gnl

# Bonus
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
	get_next_line_bonus.c get_next_line_utils_bonus.c your_main.c -o gnl_bonus
```

If `BUFFER_SIZE` is not passed on the command line, the header defaults to `10`.

### Usage example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
```

Bonus idea: open several files and alternate calls to `get_next_line(fd1)`, `get_next_line(fd2)`, etc. — each FD keeps its own reading position / leftover buffer.

### Turn-in files

**Mandatory**

- `get_next_line.c`
- `get_next_line_utils.c`
- `get_next_line.h`

**Bonus**

- `get_next_line_bonus.c`
- `get_next_line_bonus.h`
- `get_next_line_utils_bonus.c`

---

## Resources

### Documentation & references

- [read(2) — Linux man page](https://man7.org/linux/man-pages/man2/read.2.html)
- [malloc(3) / free(3)](https://man7.org/linux/man-pages/man3/malloc.3.html)
- [Static variables in C (cppreference / tutorials)](https://en.cppreference.com/w/c/language/storage_duration) — storage duration and static locals
- [File descriptors overview](https://en.wikipedia.org/wiki/File_descriptor)
- 42 subject *Get Next Line* (intra) — official specifications, including `BUFFER_SIZE` and bonus rules

### How AI was used

AI assistance (Cursor) was used **only as a support tool**, not as a substitute for understanding the project:

| Task | Scope |
|------|--------|
| README | Drafting and structuring this `README.md` to match curriculum requirements |
| Clarifications | Occasional help on concepts (static variables, buffer/remainder logic) when stuck |

The implementation of `get_next_line` (algorithm, memory management, bonus multi-FD design) was written and validated through personal work, testing, and peer learning, in line with 42’s AI guidelines for foundational projects.
