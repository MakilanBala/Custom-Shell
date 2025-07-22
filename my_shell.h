#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 128
#define MAX_CMD_LENGTH 1024


char **parse_input(char *line);

void free_tokens(char **tokens);

int shell_builtins(char **args, char **env, char *initial_directory);

//built-in commands like cd, pwd, exit, help, etc.
int command_cd(char **args, char *initial_directory);
int command_pwd();
int command_echo(char **args);
int command_env(char **env);
int command_which(char **args, char **env);

char **command_setenv(char **args, char **env);
char **command_unsetenv(char **args, char **env);

//executor function to run external commands
int executor(char **args, char **env);
int child_process(char **args, char **env);

//Path related functions
char *get_path(char **env);
char **split_paths(char *paths, int *count);