// fis.h
#include <stdlib.h>
#include <stdbool.h>

#ifndef FIS_H
#define FIS_H
typedef struct {
    bool is_dir;
    bool recursive;
    char **filepaths;
    size_t size;
} FIS;

FIS *newFIS(const char *path, bool recursive);
void freeFIS(FIS *fs);
#endif //FIS_H
