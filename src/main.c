// version 1.0
#include "asmx.h"
int main(void) {
    const char *file = "sample.yaml";
    ASMX *asmx = newAsmx(file);
    ASMC *asmc = newAsmc(asmx);

    build(asmc);

    freeAsmx(asmx);
    freeAsmc(asmc);
    return 0;
}
