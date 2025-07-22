#include "my_shell.h"
#include <string.h>

// executes a command by forking and running it in a child process
int executor(char **args, char **env) {
    pid_t pid; 
    int status;

    pid = fork(); // create a new process

    if (pid == -1) {
        perror("fork");
        exit(1); // return 1 if fork failed
    }

    if (pid == 0) {
        if (child_process(args, env)) {
            return 1; // exit with failure if child_process fails
        } // child process executes the command
    } else {
        if(waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return 1; // return 1 if waitpid fails
        }
        if (WIFSIGNALED(status)) {
            printf("Process terminated by signal %d\n", WTERMSIG(status));
        }
    }
    return 0; // return 0 if command executed successfully

}

int child_process(char **args, char **env) {
    (void) env; // unused parameter
    if (execvp(args[0], args) == -1) {
        perror("execvp");
        return 1; // return 1 if execvp fails
    }
    return 0; // return 0 if execvp succeeds
}


