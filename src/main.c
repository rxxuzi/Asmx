// version 1.0
#include <stdio.h>
#include <stdlib.h>
#include "asmx.h"
int main(const int argc, const char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    const char *file = argv[1];

    ASMX *asmx = newAsmx(file);
    ASMC *asmc = newAsmc(asmx);

    build(asmc);

    freeAsmx(asmx);
    freeAsmc(asmc);
    return 0;
}
