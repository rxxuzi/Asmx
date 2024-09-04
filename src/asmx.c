#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "fis.h"
#include "asmx.h"
#include "cson/cson.h"
#include "cson/csonx.h"
#include "utils/console.h"

void initAsmx(ASMX *asmx) {
    strcpy(asmx->projectName, "");
    asmx->sources = NULL;
    asmx->numSources = 0;
    asmx->ignores = NULL;
    asmx->numIgnores = 0;
    asmx->libraries = NULL;
    asmx->numLibraries = 0;
}

void addSource(ASMX *asmx, const char *sourcePath) {
    char **temp = realloc(asmx->sources, (asmx->numSources + 1) * sizeof(char *));
    if (temp != NULL) {
        asmx->sources = temp;
        asmx->sources[asmx->numSources] = strdup(sourcePath);
        asmx->numSources++;
    } else {
        fprintf(stderr, "Error reallocating memory for sources\n");
    }
}

void addIgnore(ASMX *asmx, const char *path) {
    char **temp = realloc(asmx->ignores, (asmx->numIgnores + 1) * sizeof(char *));
    if (temp != NULL) {
        asmx->ignores = temp;
        asmx->ignores[asmx->numIgnores] = strdup(path);
        asmx->numIgnores++;
    } else {
        fprintf(stderr, "Error reallocating memory for ignores\n");
    }
}

void addLibrary(ASMX *asmx, const char *libraryPath) {
    char **temp = realloc(asmx->libraries, (asmx->numLibraries + 1) * sizeof(char *));
    if (temp != NULL) {
        asmx->libraries = temp;
        asmx->libraries[asmx->numLibraries] = strdup(libraryPath);
        asmx->numLibraries++;
    } else {
        fprintf(stderr, "Error reallocating memory for libraries\n");
    }
}

void freeAsmx(ASMX *asmx) {
    for (int i = 0; i < asmx->numSources; i++) {
        free(asmx->sources[i]);
    }
    free(asmx->sources);
}


ASMX *newAsmx(const char *filename) {
    JsonValue* json = read_json(filename);
    if (!json) {
        fprintf(stderr, "Failed to parse JSON file: %s\n", filename);
        return NULL;
    }

    JsonObject* root = json_value_as_object(json);
    if (!root) {
        fprintf(stderr, "Root of JSON is not an object\n");
        free_json_value(json);
        return NULL;
    }

    ASMX *asmx = malloc(sizeof(ASMX));
    initAsmx(asmx);

    const char* project_name = json_object_get_string(root, "project", "");
    strncpy(asmx->projectName, project_name, sizeof(asmx->projectName) - 1);

    JsonArray* sources = json_object_get_array(root, "source");
    if (sources) {
        for (int i = 0; i < json_array_size(sources); i++) {
            JsonValue* source = json_array_get(sources, i);
            if (source && source->type == JSON_STRING) {
                struct stat st;
                if (stat(source->value.string, &st) == 0) {
                    if (S_ISDIR(st.st_mode)) {
                        FIS *fs = newFIS(source->value.string, false);
                        for (size_t j = 0; j < fs->size; j++) {
                            addSource(asmx, fs->filepaths[j]);
                        }
                        freeFIS(fs);
                    } else {
                        addSource(asmx, source->value.string);
                    }
                }
            }
        }
    }

    JsonArray* ignores = json_object_get_array(root, "ignore");
    if (ignores) {
        for (int i = 0; i < json_array_size(ignores); i++) {
            JsonValue* ignore = json_array_get(ignores, i);
            if (ignore && ignore->type == JSON_STRING) {
                struct stat st;
                if (stat(ignore->value.string, &st) == 0) {
                    if (S_ISDIR(st.st_mode)) {
                        FIS *fs = newFIS(ignore->value.string, false);
                        for (size_t j = 0; j < fs->size; j++) {
                            addIgnore(asmx, fs->filepaths[j]);
                        }
                        freeFIS(fs);
                    } else {
                        addIgnore(asmx, ignore->value.string);
                    }
                }
            }
        }
    }

    JsonArray* libraries = json_object_get_array(root, "libraries");
    if (libraries) {
        for (int i = 0; i < json_array_size(libraries); i++) {
            JsonValue* library = json_array_get(libraries, i);
            if (library && library->type == JSON_STRING) {
                addLibrary(asmx, library->value.string);
            }
        }
    }

    free_json_value(json);
    return asmx;
}