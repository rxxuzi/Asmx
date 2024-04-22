#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "options.h"
void gen();

Opt *newOpt() {
    Opt *option = malloc(sizeof(Opt));
    option->run = false;
    option->clean = false;
    option->detail = false;
    option->output = NULL;
    option->input = NULL;
    option->optimization = 0;
    option->build_type = TYPE_BUILD;
    return option;
}

void freeOpt(Opt *opt) {
    free(opt->input);
    free(opt->output);
    free(opt);
}

void immediate_opt(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i],"--version") == 0) {
            printf("Version %s\n", VERSION);
            exit(0);
        } else if (strcmp(argv[i], "--gen") == 0) {
            gen();
            exit(0);
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help();
            exit(0);
        }
    }
}

void config_opt(int argc, char *argv[], Opt *o) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--run") == 0 || strcmp(argv[i], "-r") == 0) {
            o->run = true;
        } else if (strcmp(argv[i], "--compile") == 0 || strcmp(argv[i], "-c") == 0) {
            o->build_type = TYPE_COMPILE_ONLY;
        } else if (strcmp(argv[i], "--detail") == 0 || strcmp(argv[i], "-d") == 0) {
            o->detail = true;
        } else if (strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                o->output = argv[i + 1];
            }
            ++i;
        } else if (strcmp(argv[i], "-x") == 0) {
            o->clean = true;
        }

        else {
            if (!o->input) {
                o->input = argv[i];
            }
        }
    }
}

void gen() {
    FILE *fp = fopen("build.yaml", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(fp, "!asmx\n");
    fprintf(fp, "project : a\n");
    fprintf(fp, "sources :\n");
    fprintf(fp, "  - .\n");
    fprintf(fp, "ignore :\n");
    fprintf(fp, "  -\n");
    fprintf(fp, "libraries :\n");
    fprintf(fp, "  -\n");
    fclose(fp);

    printf("build.yaml generated.\n");
}

void help() {
    printf("asmx %s - x64 Windows NASM+C Build Tool\n", VERSION);
    printf("Usage: asmx [options] [file]\n");
    printf("\nOptions:\n");
    printf("  -v, --version       Print the version number and exit.\n");
    printf("  --gen               Generate default asmx-build.yaml and exit.\n");
    printf("  -h, --help          Print this help message and exit.\n");
    printf("  -r, --run           Execute the compiled executable after building.\n");
    printf("  -c, --compile       Compile only; do not link.\n");
    printf("  -x,                 Clean the build directory.\n");
    printf("  -d, --detail        Display detailed build information.\n");
    printf("  -o, --output <file> Specify the output executable name.\n");
    printf("\nUse 'asmx [options] <build.yaml>' to specify a build configuration file.\n");
    printf("If no file is specified, 'asmx-build.yaml' is assumed.\n");
}
