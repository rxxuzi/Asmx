#include "cache.h"
#include "util.h"
#include "utils/hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>


void printCache(Cache *cache) {
    printf("stat: %c\n", cache->stat);
    printf("name: %s\n", cache->name);
    printf("hash: %s\n", cache->hash);
    printf("time: %s\n", cache->time);
    printf("type: %c\n", cache->type);
    printf("size: %ld\n", cache->size);
}

void printCaches(Caches *caches) {
    for (int i = 0; i < caches->length; i++) {
        printCache(caches->cache[i]);
    }
}


void initCache(Cache *cache) {
    cache->stat = '?';
    cache->name = NULL;
    cache->hash = NULL;
    cache->time = NULL;
    cache->type = 0;
    cache->size = 0;
}

Cache *newCache(char *filename) {
    if (!file_exists(filename)) {
        return NULL;
    }

    struct stat st;
    if (stat(filename, &st) != 0) {
        return NULL;
    }

    Cache *cache = malloc(sizeof(Cache));
    if (!cache) {
        return NULL;
    }
    initCache(cache);

    cache->name = strdup(filename);
    if (!cache->name) {
        free(cache);
        return NULL;
    }

    cache->hash = file_hash(filename);
    if (!cache->hash) {
        free(cache->name);
        free(cache);
        return NULL;
    }

    char *time_str = ctime(&st.st_mtime);
    if (!time_str) {
        free(cache->hash);
        free(cache->name);
        free(cache);
        return NULL;
    }

    time_str[strcspn(time_str, "\n")] = '\0';

    cache->time = strdup(time_str);

    cache->type = S_ISREG(st.st_mode) ? 't' : 'b';
    cache->size = st.st_size;
    cache->stat = '+';

    return cache;
}

Caches *newCaches(char **filenames, int length) {
    Caches *caches = malloc(sizeof(Caches));
    if (!caches) {
        perror("Failed to allocate memory for Caches");
        return NULL;
    }

    caches->cache = malloc(sizeof(Cache *) * length);
    if (!caches->cache) {
        perror("Failed to allocate memory for Cache pointers");
        free(caches);
        return NULL;
    }

    caches->length = length;
    for (int i = 0; i < length; i++) {
        caches->cache[i] = newCache(filenames[i]);
    }
    return caches;
}


void freeCache(Cache *cache) {
    free(cache->hash);
    free(cache->name);
    free(cache->time);
    free(cache);
}

void freeCaches(Caches *caches) {
    for (int i = 0; i < caches->length; i++) {
        freeCache(caches->cache[i]);
    }
    free(caches);
}

void saveCaches(Caches *caches, char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file for writing");
        return;
    }

    if (fwrite(&caches->length, sizeof(int), 1, fp) != 1) {
        perror("Failed to write the length of caches");
        fclose(fp);
        return;
    }

    for (int i = 0; i < caches->length; i++) {
        if (fwrite(caches->cache[i], sizeof(Cache), 1, fp) != 1) {
            perror("Failed to write cache object");
            fclose(fp);
            return;
        }
    }

    fclose(fp);
    printf("Caches saved successfully.\n");
}


Caches *loadCaches(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open file for reading");
        return NULL;
    }

    Caches *caches = malloc(sizeof(Caches));
    if (!caches) {
        fclose(fp);
        perror("Failed to allocate memory for Caches structure");
        return NULL;
    }

    if (fread(&caches->length, sizeof(int), 1, fp) != 1) {
        free(caches);
        fclose(fp);
        perror("Failed to read the length of caches");
        return NULL;
    }

    caches->cache = malloc(sizeof(Cache*) * caches->length);
    if (!caches->cache) {
        free(caches);
        fclose(fp);
        perror("Failed to allocate memory for cache pointers");
        return NULL;
    }

    for (int i = 0; i < caches->length; i++) {
        caches->cache[i] = malloc(sizeof(Cache));
        if (!caches->cache[i]) {
            // If allocation fails, clean up previously allocated caches
            for (int j = 0; j < i; j++) {
                free(caches->cache[j]);
            }
            free(caches->cache);
            free(caches);
            fclose(fp);
            perror("Failed to allocate memory for a Cache object");
            return NULL;
        }

        if (fread(caches->cache[i], sizeof(Cache), 1, fp) != 1) {
            for (int j = 0; j <= i; j++) {
                free(caches->cache[j]);
            }
            free(caches->cache);
            free(caches);
            fclose(fp);
            perror("Failed to read a Cache object");
            return NULL;
        }
    }

    printCaches(caches);

    fclose(fp);
    return caches;
}

void compareCaches(Caches *old, Caches *new) {

    int *oldFlag = calloc(old->length, sizeof(int));
    int *newFlag = calloc(new->length, sizeof(int));

    // 古いキャッシュと新しいキャッシュを比較
    for (int i = 0; i < old->length; i++) {
        Cache *oldCache = old->cache[i];
        for (int j = 0; j < new->length; j++) {
            Cache *newCache = new->cache[j];
            if (strcmp(oldCache->name, newCache->name) == 0) {
                if (strcmp(oldCache->hash, newCache->hash) == 0) {
                    newCache->stat = '=';  // 変更なし
                    oldFlag[i] = 1;
                    newFlag[j] = 1;
                } else {
                    newCache->stat = '!';  // 内容が変更された
                    oldFlag[i] = 1;
                    newFlag[j] = 1;
                }
            } else if (strcmp(oldCache->hash, newCache->hash) == 0) {
                newCache->stat = '*';  // 名前が変更された
                oldFlag[i] = 1;
                newFlag[j] = 1;
            }
        }
    }

    for (int i = 0; i < old->length; i++) {
        if (!oldFlag[i]) {
            old->cache[i]->stat = '-';  // 削除された
        }
    }

    for (int j = 0; j < new->length; j++) {
        if (!newFlag[j]) {
            new->cache[j]->stat = '+';  // 追加された
        }
    }

    free(oldFlag);
    free(newFlag);
}
