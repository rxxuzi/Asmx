// options.h
#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

#define VERSION "2.0"

typedef enum {
    CMD_UNKNOWN,
    CMD_BUILD,
    CMD_COMPILE,
    CMD_RUN,
    CMD_CLEAN,
    CMD_HELP,
    CMD_VERSION,
    CMD_GEN
} Command;

typedef struct {
    Command command;
    char *input_file;
    char *output_file;
    bool detail;
} Options;

Options* parse_options(int argc, char **argv);
void free_options(Options *opt);
void print_help();
void print_version();
void generate_default_config();

#endif // OPTIONS_H