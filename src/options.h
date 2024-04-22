#include <stdbool.h>
#ifndef OPTIONS_H
#define OPTIONS_H

#define VERSION "1.1"
#define TYPE_BUILD 0
#define TYPE_COMPILE_ONLY 1

typedef struct {
    bool run;
    bool clean;
    bool detail;
    int build_type;
    int optimization;
    char *output;
    char *input;
}Opt;

Opt *newOpt();
void help();
void freeOpt(Opt *opt);
void immediate_opt(int argc, char *argv[]);
void config_opt(int argc, char *argv[], Opt *o);
#endif //OPTIONS_H
