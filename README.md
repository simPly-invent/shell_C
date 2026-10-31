*This project has been created as part of the 42 curriculum by mobenais.*

# Minishell

## Description
Minishell is a simplified version of the Bash shell. The goal of this project is to understand the inner workings of a shell, specifically process management, file descriptors, and environment variables.

## Instructions
### Compilation
To compile the project, use the provided Makefile:
```bash
make
```

### Execution
To run the shell:
```bash
./minishell
```

## Resources
- Bash Manual: https://www.gnu.org/software/bash/manual/
- Man pages: `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 2 dup2`
- AI Usage: AI was used to accelerate the boilerplate implementation of the AST structure and to debug memory alignment issues in the sandbox environment.
