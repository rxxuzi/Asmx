#ifndef UTIL_H
#define UTIL_H

#include "utils/console.h"
int file_exists(const char *path);
int directory_exists(const char *path);

int remove_directory(const char *path);
void refresh_dir(const char *path);
#endif //UTIL_H
