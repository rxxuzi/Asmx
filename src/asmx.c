#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "fis.h"
#include "asmx.h"

void initAsmx(ASMX *asmx) {
    strcpy(asmx->projectName, "");
    asmx->sources = NULL;
    asmx->numSources = 0;
}

void addSource(ASMX *asmx, const char *sourcePath) {
    asmx->sources = realloc(asmx->sources, (asmx->numSources + 1) * sizeof(char *));
    asmx->sources[asmx->numSources] = strdup(sourcePath);
    asmx->numSources++;
}

void freeAsmx(ASMX *asmx) {
    for (int i = 0; i < asmx->numSources; i++) {
        free(asmx->sources[i]);
    }
    free(asmx->sources);
}

ASMX *newAsmx(const char *filename) {
    FILE *fp = fopen(filename, "r");
    char line[1024];
    ASMX *asmx = malloc(sizeof(ASMX));
    initAsmx(asmx);

    if (fp == NULL) {
        perror("Failed to open file");
        free(asmx);
        return NULL;
    }

    char current_section[100] = {0};

    if (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "!asmx") == NULL) {
            fclose(fp);
            return  NULL;
        }
    }

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        if (strchr(line, ':')) {
            char *key = strtok(line, ":");
            char *value = strtok(NULL, "\n");
            while (value && *value == ' ') value++;

            if (strcmp(key, "project ") == 0) {
                strcpy(asmx->projectName, value);
            } else if (strcmp(key, "sources ") == 0 || strcmp(key, "flags ") == 0) {
                strcpy(current_section, key);
            }
        } else if (strstr(line, "  - ") && current_section[0] != '\0') {
            char *item = line + 4; // Skip "  - "
            while (*item == ' ') item++;

            if (strcmp(current_section, "sources ") == 0) {
                struct stat st;
                if (stat(item, &st) == 0 && S_ISDIR(st.st_mode)) {
                    FIS *fs = newFIS(item, false);
                    for (size_t i = 0; i < fs->size; i++) {
                        addSource(asmx, fs->filepaths[i]);
                    }
                    freeFIS(fs);
                } else {
                    if (stat(item, &st) == 0) {
                        addSource(asmx, item);
                    }
                }
            }
        }
    }

    fclose(fp);
    return asmx;
}
