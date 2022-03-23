#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LEN 10

char * line_array[ARRAY_LEN];
int idx = 0;

void initialize_line_array() {
    int i;

    for (i = 0; i < ARRAY_LEN; ++i) {
        line_array[i] = malloc(128 * sizeof(char));
    }

    idx = 0;
}

void free_line_array() {
    int i;

    for (i = 0; i < ARRAY_LEN; ++i) {
        free(line_array[i]);
    }
}

void clear_line_array() {
    idx = 0;
}

void add_to_line_array(char * token) {
    strcpy(line_array[idx], token);
    ++idx;
}

char ** get_line_array() {
    return line_array;
}

int get_line_array_len() {
    return idx + 1;
}

void print_line_array() {
    int i;

    for (i = 0; i < idx; ++i) { 
        fprintf(stdout, "%s ", line_array[i]);
    }

    fprintf(stdout, "\n");
}

