#include "my_shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_TOK_BUFSIZE 64

char **parse_input(char *line) {
    size_t bufsize = INITIAL_TOK_BUFSIZE;
    size_t position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    if (!tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    size_t i = 0;
    while (line[i]) {
        // skip any spaces
        while (line[i] && isspace((unsigned char)line[i])) {
            i++;
        }
        if (!line[i]) break;

        // grow tokens array if needed
        if (position + 1 >= bufsize) {
            bufsize *= 2;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        // check for separators ;, &&, ||
        if (line[i] == ';') {
            tokens[position] = strdup(";");
            if (!tokens[position]) { perror("strdup"); exit(EXIT_FAILURE); }
            position++;
            i++;
        }
        else if (line[i] == '&' && line[i+1] == '&') {
            tokens[position] = strdup("&&");
            if (!tokens[position]) { perror("strdup"); exit(EXIT_FAILURE); }
            position++;
            i += 2;
        }
        else if (line[i] == '|' && line[i+1] == '|') {
            tokens[position] = strdup("||");
            if (!tokens[position]) { perror("strdup"); exit(EXIT_FAILURE); }
            position++;
            i += 2;
        }
        else {
            // regular word token
            size_t start = i;
            while (line[i]
                   && !isspace((unsigned char)line[i])
                   && line[i] != ';'
                   && !(line[i] == '&' && line[i+1] == '&')
                   && !(line[i] == '|' && line[i+1] == '|')) {
                i++;
            }
            size_t len = i - start;
            tokens[position] = malloc(len + 1);
            if (!tokens[position]) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            memcpy(tokens[position], &line[start], len);
            tokens[position][len] = '\0';
            position++;
        }
    }

    tokens[position] = NULL;
    return tokens;
}

void free_tokens(char **tokens) {
    if (!tokens) return;
    for (size_t i = 0; tokens[i]; i++) {
        free(tokens[i]);
    }
    free(tokens);
}