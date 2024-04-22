#include "asmx.h"
#include "fis.h"
#include "util.h"
#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int compileAssembly(const char *filepath, const char *target_dir) {
    char command[1024];
    char objPath[1024];
    char *fileName;
    fileName = strrchr(filepath, '/');
    if (fileName == NULL) {
        fileName = strrchr(filepath, '\\');
    }
    if (fileName == NULL) {
        fileName = (char *)filepath;
    } else {
        fileName++;
    }

    snprintf(objPath, sizeof(objPath), "%s/%s", target_dir, fileName);
    char *dot = strrchr(objPath, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
    strcat(objPath, ".obj");

    snprintf(command, sizeof(command), "nasm -f win64 \"%s\" -o \"%s\"", filepath, objPath);

    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Failed to compile %s\n", filepath);
    }
    return result;
}

int compileCLang(const char *filepath, const char *target_dir) {
    char command[1024];
    char objPath[1024];
    char *fileName;

    fileName = strrchr(filepath, '/');
    if (fileName == NULL) {
        fileName = strrchr(filepath, '\\');
    }
    if (fileName == NULL) {
        fileName = (char *)filepath;
    } else {
        fileName++;
    }

    snprintf(objPath, sizeof(objPath), "%s/%s", target_dir, fileName);
    char *dot = strrchr(objPath, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
    strcat(objPath, ".o");

    snprintf(command, sizeof(command), "gcc -c \"%s\" -o \"%s\"", filepath, objPath);

    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Failed to compile %s\n", filepath);
    }
    return result;
}

int linker(ASMC *asmc, char **objectPaths, size_t objectSize) {
    char command[1024] = "gcc";

    for (size_t i = 0; i < objectSize; i++) {
        strcat(command, " ");
        strcat(command, "\"");
        strcat(command, objectPaths[i]);
        strcat(command, "\"");
    }

    for (size_t i = 0; i < asmc->asmx->numLibraries; i++) {
        strcat(command, " -l");
        strcat(command, asmc->asmx->libraries[i]);
    }

    strcat(command, " -o ");
    strcat(command, "\"");
    strcat(command, asmc->project);
    strcat(command, "\"");

    printf("%s\n", command);
    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Failed to link object files\n");
    }
    return result;
}

int build(ASMC *asmc, int type) {
    struct stat st;
    if (stat(BUILD_DIR, &st) == -1) {
        mkdir(BUILD_DIR);
    } else {
        refresh_dir(BUILD_DIR);
    }


    for (int i = 0; i < asmc->asmSize; ++i) {
        int r = compileAssembly(asmc->assembly[i], BUILD_DIR);
        if (r != 0) {
            return r;
        }
    }
    for (int i = 0; i < asmc->srcSize; ++i) {
        int r = compileCLang(asmc->source[i], BUILD_DIR);
        if (r != 0) {
            return r;
        }
    }

    if (type == TYPE_BUILD) {
        FIS *fis = newFIS(BUILD_DIR, false);
        linker(asmc, fis->filepaths, fis->size);
        freeFIS(fis);
    }

    return 0;
}


