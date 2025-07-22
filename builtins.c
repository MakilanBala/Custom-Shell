#include "my_shell.h"
#include <string.h>

int command_cd(char **args, char *initial_directory) {
    (void) initial_directory; // unused parameter

    if (args[1] == NULL) {
       printf("cd: expected argument \"cd [path]\"\n");
    } else if (chdir(args[1]) == 0) {

    } else {
        perror("cd");
    }
    
    return 0; // return 0 if command executed successfully
}


int command_pwd() {
    char *cwd = NULL;

    cwd = getcwd(NULL, 0);
    if (cwd != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd");
    }  
    return 0; // return 0 if command executed successfully
}


int command_echo(char **args) {
    
    int newline = 1; // default to printing newline
    int i = 1; // skipping the -n

    if (args[1] != NULL && strcmp(args[1], "-n") == 0) {
        newline = 0; // if -n is present, do not print newline
        i++;
    }
    //process the rest of the arguments
    for (; args[i] != NULL; i++) {
        
        if (args[i][0] == '$') {
            // if the argument starts with $, print the environment variable
            char *value = getenv(args[i] + 1); // skip the '$' and get the value
            if (value) {
                printf("%s", value);
            } else {
                printf("");
            }
        } else {
            printf("%s ", args[i]);
        }

        if (args[i + 1] != NULL) {
            newline = 0; // if there are more arguments, do not print newline
            printf(" ");
        }

        if (newline) {
            printf("\n");
        }
        newline = 1; // reset for next argument
    }

    return 0; // return 0 if command executed successfully
}


int command_env(char **env) {
    size_t index = 0;
    while (env[index]) {
        printf("%s\n", env[index]);
        index++;
    }

    return 0; // return 0 if command executed successfully
}

char* find_command_in_path(char *command, char **env);

int command_which(char **args, char **env) {
    if (args[1] == NULL) {
        printf("which: expected argument \"which [command]\"\n");
        return 1; // return 1 if no command was specified
    }


    // list of the built-in commands
    const char *builtins[] = {"cd", "pwd", "echo", "env", "which", "exit", "quit", NULL};
    
    for (size_t i = 0; builtins[i]; i++) {
        
        if (strcmp(args[1], builtins[i]) == 0) {
            printf("%s: shell built-in command\n", args[1]);
            return 0; // return 0 if the command is a built-in
        }
    }

    //check external commands
    char *full_path = find_command_in_path(args[1], env);

    if (full_path) {
        printf("%s\n", full_path);
        free(full_path); // free the allocated memory for the full path
        return 0; // return 0 if the command was found
    } else {
        printf("which: %s command not found\n", args[1]);
        return 1; // return 1 if the command was not found
    }

}

// Function to find the command in the PATH environment variable
char* find_command_in_path(char *command, char **env) {
     (void) env; // unused parameter

    char *path_env = NULL; // Store the PATH environment variable
    char *path = NULL; // Duplicate of the PATH variable
    char *token = NULL; // Tokenized dirs from PATH
    char full_path[1024]; // Buffer to construct full paths

    
    // Locate the PATH 
    
    path_env = getenv("PATH=");

    if (!path_env) {
       return NULL; // No path
    }

    // Duplicate the PATH variable avoiding modifying the original
    path = strdup(path_env);

    if (!path) {
        perror("strdup");
        return NULL; // Memory allocation failed
    }

    // Split the PATH into directories
    token = strtok(path, ":");
    while (token != NULL) {
        // construct the full path
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
        
        // Check if the command exists as executable in the directory
        if (access(full_path, X_OK) == 0) {
            free(path); // Free the duplicated PATH
            return strdup(full_path); // Return the full path of the command
        }
        token = strtok(NULL, ":"); // Get the next directory
    }

    free(path); // Free the duplicated PATH
    return NULL; // Return NULL if the command is not found
}

