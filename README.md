# mini-shell

A small Unix shell written in C from scratch, to learn how a shell like `bash` really works under the hood: processes, file descriptors and pipes.

```
> ls -l | grep txt
-rw-r--r-- 1 user user 64 Oct 11 13:14 liste.txt
> ls > files.txt
> cd /tmp
> pwd
/tmp
> exit
```

## Features

- Runs any program installed on the system, with its arguments (`ls -l`, `echo hello`, `cat file.txt`...)
- Built-in commands: `cd` and `exit`
- Output redirection: `command > file`
- Pipes: `command1 | command2`
- Combined: `command1 | command2 > file`
- Error handling: unknown commands, missing file name (`ls >`), missing command around a pipe (`ls |`, `| grep`), lines that are too long, `Ctrl+D`
- Tolerates extra spaces anywhere in the line

## Build and run

Requires `gcc` and `make` on Linux.

```bash
make
./shell
```

`make clean` removes the compiled files.

## Project structure

| File | Role |
|---|---|
| `main.c` | Main loop: read a line, parse it, run it |
| `parser.c` / `parser.h` | Split the line into words, detect `>` and `\|` |
| `executor.c` / `executor.h` | Run commands: `cd`, simple commands, pipes |
| `Makefile` | Build rules |

## How it works

1. **Reading**: the line is read with `fgets`. Lines longer than the buffer are rejected and the rest of the input is discarded.
2. **Parsing**: the line is split in place. Separators are replaced with `'\0'` and an array of pointers (`char *words[]`) points to the start of each word, terminated by `NULL`, which is the format `execvp` expects.
3. **Built-ins**: `cd` must run in the shell process itself (with `chdir`), because a child process cannot change its parent's working directory.
4. **Execution**: the shell creates a child process with `fork`. The child replaces itself with the requested program using `execvp`, while the parent waits with `waitpid`.
5. **Redirection**: before `execvp`, the child opens the file with `open` and uses `dup2` to plug its standard output (file descriptor 1) into the file.
6. **Pipes**: the shell creates a pipe with `pipe`, then two children. The first one plugs its standard output into the write end, the second one plugs its standard input (file descriptor 0) into the read end. Every unused end is closed so the second program correctly detects the end of the data.

## Limitations

- Only one pipe per command
- `>` and `|` must be surrounded by spaces (`ls > file`, not `ls>file`)
- With a pipe, the redirection only applies to the second command
- No quotes, environment variables, `>>`, `<` or background jobs (`&`)
- Input lines are limited to 99 characters

## What I learned

System programming in C on Linux: pointers and arrays of strings, process creation (`fork`, `execvp`, `waitpid`), file descriptors (`open`, `dup2`, `close`), inter-process communication with `pipe`, and organizing a C project into modules with a `Makefile`.
