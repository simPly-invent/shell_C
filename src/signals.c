# include "minishell.h"

volatile sig_atomic_t g_signal = 0;

void    handle_sigint(int sig)
{
    (void)sig;
    g_signal = SIGINT;
    write(STDOUT_FILENO, "\n", 1);
    // Note: readline functions removed since we use fgets
    // rl_on_new_line();
    // rl_replace_line("", 0);
    // rl_redisplay();
}

void    handle_sigquit(int sig)
{
    (void)sig;
    // Bash ignores SIGQUIT in the main shell
}

void    handle_heredoc_sigint(int sig)
{
    (void)sig;
    g_signal = SIGINT;
    // We don't close STDIN here as it might be the terminal
}

void    ignore_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void    restore_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
}

void    setup_signals(void)
{
    struct sigaction  sa_int;
    struct sigaction  sa_quit;

    ft_memset(&sa_int, 0, sizeof(sa_int));
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    ft_memset(&sa_quit, 0, sizeof(sa_quit));
    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = SA_RESTART;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
