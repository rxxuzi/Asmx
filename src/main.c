// version 1.0
#include <stdio.h>
#include <stdlib.h>
#include "asmx.h"

int main(const int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(1);
    }
    char *file = argv[1];

    ASMX *asmx = newAsmx(file);
    if (asmx == NULL) {
        fprintf(stderr, "Error: failed to open file '%s'\n", file);
        exit(1);
    }

    printAsmx(asmx);
    ASMC *asmc = newAsmc(asmx);

    int r;
    r = build(asmc);
    freeAsmx(asmx);
    freeAsmc(asmc);
    if (r != 0) {
        perror("Error: failed to build file");
        exit(1);
    }
    printf("Done.\n");
    return 0;
}
