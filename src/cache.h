#ifndef CACHE_H
#define CACHE_H

#define DIR_CACHE ".cache/"
#define DIR_CACHE_TMP ".cache/tmp/"
#define DIR_CACHE_LOG ".cache/log/"
#define DIR_CACHE_HISTORY ".cache/history"

typedef struct {
    char stat;
    char *name;
    char *hash;
    char *time;
    char type;
    long size;
}Cache;

typedef struct {
    int length;
    Cache **cache;
}Caches;

// functions

Cache *newCache(char *filename);
Caches *newCaches(char **filename, int length);
void freeCache(Cache *cache);
void freeCaches(Caches *caches);
void saveCaches(Caches *caches, char *filename);
Caches *loadCaches(const char *filename);
void printCache(Cache *cache);
void printCaches(Caches *caches);
void compareCaches(Caches *old, Caches *new);

#endif //CACHE_H
