*This project has been created as part of the 42 curriculum by mobenais.*

# Minishell

A small Bash-like shell written in C: a tokenizer, a parser that builds an
AST, and an executor that handles pipes, redirections, environment
expansion, quoting, builtins and signals.

![minishell session](docs/demo.gif)

## Build and run

```sh
make
./minishell
```

The only dependency is the libc plus `readline` for linking; the bundled
`libft/` provides the string helpers.

## Features

| Area           | Supported                                                              |
|----------------|------------------------------------------------------------------------|
| Builtins       | `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit [code]` |
| Pipes          | any length: `cmd1 \| cmd2 \| cmd3`                                      |
| Redirections   | `<`, `>`, `>>`, and `<<` heredoc with a delimiter                        |
| Expansion      | `$VAR` and `$?`, not expanded inside single quotes                     |
| Quotes         | single and double quotes, including in the middle of a word            |
| Path lookup    | commands resolved through `$PATH`, absolute and relative paths         |
| Signals        | `Ctrl-C` prints a new prompt, `Ctrl-\` is ignored, `Ctrl-D` exits      |

The exit status of the last command is kept in `$?`.

```
minishell$ export NAME=mobenais
minishell$ echo "user: $NAME" '$NAME'
user: mobenais $NAME
minishell$ cat << END | wc -l
> one
> two
> END
2
```

## Project layout

```
.
├── includes/minishell.h     token, AST, env and shell structures
├── src/
│   ├── main.c               entry point
│   ├── shell.c              init, read-eval loop, cleanup
│   ├── signals.c            SIGINT / SIGQUIT handling
│   ├── parser/
│   │   ├── tokenizer.c      input -> tokens (words, |, <, >, >>, <<)
│   │   ├── parser.c         tokens -> AST (commands, pipes, redirections)
│   │   └── expand.c         $VAR expansion with quote awareness
│   ├── executor/
│   │   ├── executor.c       redirections, heredocs, pipelines, dispatch
│   │   ├── execve.c         PATH resolution and external commands
│   │   └── builtins_*.c     one file per builtin
│   └── utils/               environment list, free helpers
├── libft/                   string library used by the shell
└── Makefile
```

## How a line is executed

1. **Tokenize**: the line is split into words and operators, remembering
   which quotes wrapped each word.
2. **Parse**: tokens become an AST where `a | b | c` is
   `PIPE(a, PIPE(b, c))` and each command node carries its redirection list.
3. **Expand**: `$VAR` is replaced in every word that was not single-quoted.
4. **Execute**: heredocs are read first, then each pipeline stage runs in a
   child process with its stdin/stdout wired to pipes. Builtins run in the
   parent when they are not part of a pipeline, so `cd` and `export` affect
   the shell itself.

## Resources

- Bash manual: https://www.gnu.org/software/bash/manual/
- `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 2 dup2`
- AI usage: AI was used for the AST boilerplate, to debug memory issues, and
  for the pipeline and heredoc fixes in the executor.
