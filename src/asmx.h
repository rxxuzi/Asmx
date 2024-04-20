#ifndef ASMX_H
#define ASMX_H

#define BUILD_DIR "asmx-build/"

typedef struct {
    char projectName[100];
    const char **sources;
    int numSources;
} ASMX;

typedef struct {
    char *project;
    char **source;
    char **assembly;
    char **object;
    char **header;
    char **other;
    int srcSize;
    int asmSize;
    int objSize;
    int headerSize;
    int otherSize;
} ASMC;

ASMX *newAsmx(const char *filename);
ASMC *newAsmc(ASMX *asmx);
void freeAsmc(ASMC *asmc);
void freeAsmx(ASMX *asmx);
void printAsmc(ASMC *asmc);
void build(ASMC *asmc);

#endif //ASMX_H
