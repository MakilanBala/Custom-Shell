#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "my_shell.h"

#define MAX_ARGS 128

#define MAX_CMD_LENGTH 1024

//cd, pwd, exit, help, etc.
int shell_builtins(char **args, char **env, char *initial_directory) {

    if (!strcmp(args[0], "cd")) {
        return command_cd(args, initial_directory);
    } else if (!strcmp(args[0], "pwd")) {
        return command_pwd();
    } else if (!strcmp(args[0], "echo")) {
       return (command_echo(args));
    } else if (!strcmp(args[0], "env")) {
       return command_env(env);
    } else if (!strcmp(args[0], "which")) {
       return command_which(args, env);
    } else if (!strcmp(args[0], "exit") || !strcmp(args[0], "quit")) {
        exit(EXIT_SUCCESS);
    } else {

        // not a built-in command, exec
        return executor(args, env);
    }

    return 0; // return 0 if no built-in command was executed
}

void loop(char **env) {
    char *line = NULL;
    size_t len = 0;

    char **args;

    char *initial_directory = getcwd(NULL, 0);

    while (true) {

        printf("mash$ ");
        fflush(stdout);
        if (getline(&line, &len, stdin) == -1) { 
            perror("getline");
            break; // EOF , ctrl-D
        }
        line[strcspn(line, "\r\n")] = '\0';


     

    
        args = parse_input(line);

        // 1) walk through args[], splitting on ";"
        int cmd_start = 0;
        int prev_status = 0;
        char *prev_op = ";";

        while (args[cmd_start]) {
            // find end of this sub‐command (stop at ;, && or ||)
            int cmd_end = cmd_start;
            while (args[cmd_end]
               && strcmp(args[cmd_end], ";")
               && strcmp(args[cmd_end], "&&")
               && strcmp(args[cmd_end], "||")) {
            cmd_end++;
            }

            // temporarily cut off at the operator
            char *sep = args[cmd_end];
            args[cmd_end] = NULL;

            // decide if we should run this command based on prev_op and prev_status
            bool do_run = false;
            if (cmd_start == 0) {
                do_run = true;             // always run first command
            } else if (strcmp(prev_op, "&&") == 0 && prev_status == 0) {
                do_run = true;             // &&: only if previous succeeded
            } else if (strcmp(prev_op, "||") == 0 && prev_status != 0) {
                do_run = true;             // ||: only if previous failed
            } else if (strcmp(prev_op, ";") == 0) {
                do_run = true;             // ;: always run
            }

            // execute if allowed
            if (do_run && args[cmd_start]) {
                prev_status = shell_builtins(&args[cmd_start], env, initial_directory);
            }

            // restore and prepare for next
            args[cmd_end] = sep;
            if (!sep) {
                break;  // end of args[]
            }
            prev_op = sep;
            cmd_start = cmd_end + 1;
        }

        free_tokens(args);
    }

    free(line);
    free_tokens(args);
}


int main(int argc, char **argv, char **env) {
    
    (void)argc;
    (void)argv;

    loop(env);

    return 0;
}