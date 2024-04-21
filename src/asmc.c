#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asmx.h"

static void initASMC(ASMC *asmc) {
    asmc->project = NULL;
    asmc->source = NULL;
    asmc->srcSize = 0;
    asmc->assembly = NULL;
    asmc->asmSize = 0;
    asmc->object = NULL;
    asmc->objSize = 0;
    asmc->header = NULL;
    asmc->headerSize = 0;
    asmc->other = NULL;
    asmc->otherSize = 0;
}

bool isIgnored(ASMX *asmx, const char *path) {
    for (int i = 0; i < asmx->numIgnores; ++i) {
        if (strcmp(asmx->ignores[i], path) == 0) {
            printf("Ignored: %s\n", path);
            return true;
        }
    }
    return false;
}

static void addFile(const char *path, char ***array, int *size) {
    char **temp = realloc(*array, (*size + 1) * sizeof(char *));
    if (temp != NULL) {
        *array = temp;
        (*array)[*size] = strdup(path);
        (*size)++;
    } else {
        fprintf(stderr, "Failed to reallocate memory\n");
    }
}


void freeAsmc(ASMC *asmc) {
    for (int i = 0; i < asmc->srcSize; i++) free(asmc->source[i]);
    free(asmc->source);
    for (int i = 0; i < asmc->asmSize; i++) free(asmc->assembly[i]);
    free(asmc->assembly);
    for (int i = 0; i < asmc->objSize; i++) free(asmc->object[i]);
    free(asmc->object);
    for (int i = 0; i < asmc->headerSize; i++) free(asmc->header[i]);
    free(asmc->header);
    for (int i = 0; i < asmc->otherSize; i++) free(asmc->other[i]);
    free(asmc->other);
}

void addFilePath(ASMX *asmx, ASMC *asmc, const char *path) {
    const char *ext = strrchr(path, '.');
    if (ext != NULL && !isIgnored(asmx, path)) {
        if (strcmp(ext, ".c") == 0) {
            addFile(path, &asmc->source, &asmc->srcSize);
        } else if (strcmp(ext, ".asm") == 0) {
            addFile(path, &asmc->assembly, &asmc->asmSize);
        } else if (strcmp(ext, ".o") == 0 || strcmp(ext, ".obj") == 0) {
            addFile(path, &asmc->object, &asmc->objSize);
        } else if (strcmp(ext, ".h") == 0) {
            addFile(path, &asmc->header, &asmc->headerSize);
        } else {
            addFile(path, &asmc->other, &asmc->otherSize);
        }
    }
}

void addAllASMC(ASMX *asmx, ASMC *asmc, const char **paths, int size) {
    for (int i = 0; i < size; ++i) {
        addFilePath(asmx, asmc, paths[i]);
    }
}

ASMC *newAsmc(ASMX *asmx) {
    ASMC *asmc = malloc(sizeof(ASMC));
    initASMC(asmc);
    asmc->project = asmx->projectName;
    const char **paths = asmx->sources;
    int size = asmx->numSources;
    addAllASMC(asmx, asmc, paths, size);
    return asmc;
}

void printAsmc(ASMC *asmc) {
    puts("Source");
    for (int i = 0; i < asmc->srcSize; ++i) {
        printf(" - %s\n", asmc->source[i]);
    }

    puts("Assembly");
    for (int i = 0; i < asmc->asmSize; ++i) {
        printf(" - %s\n", asmc->assembly[i]);
    }

    puts("Object");
    for (int i = 0; i < asmc->objSize; ++i) {
        printf(" - %s\n", asmc->object[i]);
    }
    puts("Header");
    for (int i = 0; i < asmc->headerSize; ++i) {
        printf(" - %s\n", asmc->header[i]);
    }
}
