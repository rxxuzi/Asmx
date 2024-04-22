// version 1.2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asmx.h"
#include "options.h"
#include "util.h"

int main(const int argc, char **argv) {
    if (argc < 2) {
        help();
        exit(1);
    }

    immediate_opt(argc, argv);
    Opt *opt = newOpt();
    config_opt(argc, argv, opt);

    char *file = opt->input;

    ASMX *asmx = newAsmx(file);
    if (asmx == NULL) {
        fprintf(stderr, "Error: failed to open file '%s'\n", file);
        exit(1);
    }

    if (opt->output != NULL) {
        strncpy(asmx->projectName, opt->output, sizeof(asmx->projectName) - 1);
        asmx->projectName[sizeof(asmx->projectName) - 1] = '\0';
    }

    ASMC *asmc = newAsmc(asmx);
    if (opt->detail) {
        printAsmc(asmc);
    }

    int r = build(asmc, opt->build_type);

    if (r != 0) {
        perror("Error: failed to build file");
        exit(1);
    }

    if (opt->run) {
        system(asmx->projectName);
    }

    if (opt->clean) {
        remove_directory(BUILD_DIR);
    }

    freeOpt(opt);
    freeAsmx(asmx);
    freeAsmc(asmc);
    return 0;
}
