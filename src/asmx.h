#ifndef ASMX_H
#define ASMX_H

#define BUILD_DIR "asmx-build/"

#define ASMX_BUILD 0
#define ASMX_COMPILE 1

typedef struct {
    char projectName[100];
    char **sources;
    int numSources;
    char **ignores;
    int numIgnores;
    char **libraries;
    int numLibraries;
} ASMX;

typedef struct {
    char *project;
    char **source;
    char **assembly;
    char **object;
    char **header;
    char **other;
    char **ignore;
    int srcSize;
    int asmSize;
    int objSize;
    int headerSize;
    int otherSize;
    int ignoreSize;
    ASMX *asmx;
} ASMC;

ASMX *newAsmx(const char *filename);
ASMC *newAsmc(ASMX *asmx);
void freeAsmc(ASMC *asmc);
void freeAsmx(ASMX *asmx);
void printAsmc(ASMC *asmc);
int build(ASMC *asmc, int type);

#endif //ASMX_H
