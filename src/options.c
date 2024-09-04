// options.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "options.h"
#include "cson/cson.h"
#include "cson/csonx.h"
#include "utils/console.h"

Options* parse_options(int argc, char **argv) {
    Options *opt = calloc(1, sizeof(Options));
    opt->command = CMD_UNKNOWN;
    opt->input_file = "asmx.json";

    // Check for -h and -v first
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            opt->command = CMD_HELP;
            return opt;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            opt->command = CMD_VERSION;
            return opt;
        }
    }

    // Parse command
    if (strcmp(argv[1], "build") == 0) opt->command = CMD_BUILD;
    else if (strcmp(argv[1], "compile") == 0) opt->command = CMD_COMPILE;
    else if (strcmp(argv[1], "run") == 0) opt->command = CMD_RUN;
    else if (strcmp(argv[1], "clean") == 0) opt->command = CMD_CLEAN;
    else if (strcmp(argv[1], "help") == 0) opt->command = CMD_HELP;
    else if (strcmp(argv[1], "version") == 0) opt->command = CMD_VERSION;
    else if (strcmp(argv[1], "gen") == 0) opt->command = CMD_GEN;

    // Parse options
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) opt->output_file = argv[++i];
        } else if (strcmp(argv[i], "-d") == 0) {
            opt->detail = true;
        } else if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 < argc) opt->input_file = argv[++i];
        }
    }

    // Set default input file if not provided
    if (opt->input_file == NULL && (opt->command == CMD_BUILD || opt->command == CMD_COMPILE || opt->command == CMD_RUN)) {
        opt->input_file = "asmx.json";
    }

    return opt;
}

void free_options(Options *opt) {
    free(opt);
}

void print_help() {
    printf("asmx %s - x64 Windows NASM+C Build Tool\n", VERSION);
    printf("\nUsage: \n\n");
    printf(CYAN("    asmx <command> [options] [file]\n"));
    printf("\nCommands:\n");
    printf("  build               Build the project\n");
    printf("  compile             Compile only; do not link\n");
    printf("  run                 Build and run the project\n");
    printf("  clean               Clean the build directory\n");
    printf("  gen                 Generate default asmx.json\n");
    printf("  help                Display this help message\n");
    printf("  version             Display version information\n");
    printf("\nOptions:\n");
    printf("  -o <file>           Specify the output file name\n");
    printf("  -d                  Display detailed build information\n");
    printf("  -i <file>           Specify a custom build configuration file\n");
}

void print_version() {
    printf("asmx version %s\n", VERSION);
}

void generate_default_config() {
    JsonValue* root = json_create_object();

    JsonValue* project = json_create_string("a");
    json_object_set(root->value.object, "project", project);

    JsonValue* sources = json_create_array();
    JsonValue* source = json_create_string(".");
    json_array_append(sources->value.array, source);
    json_object_set(root->value.object, "source", sources);

    JsonValue* ignores = json_create_array();
    json_object_set(root->value.object, "ignore", ignores);

    JsonValue* libraries = json_create_array();
    json_object_set(root->value.object, "libraries", libraries);

    write_json(root, "asmx.json", 2);  // 2 spaces for indentation
    free_json_value(root);

    printf("asmx.json generated.\n");
}