#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <errno.h>
# include <termios.h>

# include "libft.h"

// Token types
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC,
    TOKEN_END
}   t_token_type;

// Token structure
typedef enum e_quote_type
{
    QUOTE_NONE,
    QUOTE_SINGLE,
    QUOTE_DOUBLE
}   t_quote_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    t_quote_type    quote_type;
    struct s_token  *next;
}   t_token;

// AST node types
typedef enum e_ast_type
{
    AST_CMD,
    AST_PIPE,
    AST_REDIR_IN,
    AST_REDIR_OUT,
    AST_REDIR_APPEND,
    AST_HEREDOC
}   t_ast_type;

// Redirection structure
typedef struct s_redir
{
    t_ast_type      type;
    char            *file;
    char            *delimiter;
    int             heredoc_fd;
    struct s_redir  *next;
}   t_redir;

// AST node structure
typedef struct s_ast
{
    t_ast_type      type;
    char            **args;
    t_quote_type    *quote_types;
    t_redir         *redirs;
    struct s_ast    *left;
    struct s_ast    *right;
}   t_ast;

// Environment structure
typedef struct s_env
{
    char            *value;
    struct s_env    *next;
}   t_env;

// Shell state structure
typedef struct s_shell
{
    t_env           *env;
    char            **env_array;
    int             last_exit_code;
    int             should_exit;
    t_token         *tokens;
    t_ast           *ast;
    char            *old_pwd;
    struct termios  original_termios;
}   t_shell;

// Global signal variable (only signal number allowed)
extern volatile sig_atomic_t g_signal;

// Main shell functions
int             main(int argc, char **argv, char **envp);
void            shell_init(t_shell *shell, char **envp);
void            shell_loop(t_shell *shell);
void            shell_cleanup(t_shell *shell);

// Signal handling
void            setup_signals(void);
void            handle_sigint(int sig);
void            handle_sigquit(int sig);
void            handle_heredoc_sigint(int sig);
void            ignore_signals(void);
void            restore_signals(void);

// Tokenizer
t_token         *tokenize(char *input);
void            free_tokens(t_token *tokens);
t_token         *create_token(t_token_type type, char *value, t_quote_type quote_type);
void            add_token(t_token **head, t_token *new);
int             is_metachar(char c);
int             is_quote(char c);

// Parser
t_ast           *parse(t_token *tokens);
t_ast           *parse_pipeline(t_token **tokens);
t_ast           *parse_command(t_token **tokens);
t_ast           *parse_redirection(t_token **tokens);
void            free_ast(t_ast *ast);

// Expander
char            *expand_variables(char *str, t_shell *shell);
char            *expand_word(char *word, t_shell *shell, t_quote_type quote_type);
char            *get_env_value(t_shell *shell, char *key);
void            expand_ast(t_ast *ast, t_shell *shell);

// Executor
int             execute_ast(t_ast *ast, t_shell *shell);
int             execute_command(t_ast *ast, t_shell *shell);
int             execute_pipeline(t_ast *ast, t_shell *shell);
int             execute_redirections(t_redir *redirs, t_shell *shell);
int             setup_heredoc(t_redir *redir, t_shell *shell);
char            *find_executable(char *cmd, t_shell *shell);
int             execute_external(t_ast *ast, t_shell *shell);

// Builtins
int             is_builtin(char *cmd);
int             execute_builtin(t_ast *ast, t_shell *shell);
int             builtin_echo(t_ast *ast, t_shell *shell);
int             builtin_cd(t_ast *ast, t_shell *shell);
int             builtin_pwd(t_ast *ast, t_shell *shell);
int             builtin_export(t_ast *ast, t_shell *shell);
int             builtin_unset(t_ast *ast, t_shell *shell);
int             builtin_env(t_ast *ast, t_shell *shell);
int             builtin_exit(t_ast *ast, t_shell *shell);

// Environment
t_env           *copy_env(char **envp);
void            free_env(t_env *env);
void            sort_env(t_env **env);
char            **get_env_array(t_env *env);
t_env           *find_env(t_env *env, char *key);
int             set_env(t_shell *shell, char *key, char *value);
int             unset_env(t_shell *shell, char *key);
void            print_env(t_env *env);

// Utilities
char            *ft_strjoin_free(char *s1, char *s2);
char            *ft_strjoin3_free(char *s1, char *s2, char *s3);
int             ft_strcmp(const char *s1, const char *s2);
void            print_error(char *cmd, char *msg);
void            print_error2(char *cmd, char *arg, char *msg);

char    **ft_realloc_args(char **args, int count, char *val);

void            add_env(t_env **env, char *key, char *value);

char    *ft_strtok(char *s, const char *delim);

char    *get_current_cwd(void);
#endif
