// main.c
#include <stdio.h>
#include <stdlib.h>
#include "asmx.h"
#include "options.h"
#include "util.h"

int main(int argc, char **argv) {
    evtp();
    if (argc < 2) {
        print_help();
        return 0;
    }

    Options *opt = parse_options(argc, argv);

    switch (opt->command) {
        case CMD_BUILD:
        case CMD_COMPILE:
        case CMD_RUN:
        {
            ASMX *asmx = newAsmx(opt->input_file);
            if (asmx == NULL) {
                printf("Error: failed to open file '%s'\n", opt->input_file);
                exit(1);
            }

            if (opt->output_file != NULL) {
                strncpy(asmx->projectName, opt->output_file, sizeof(asmx->projectName) - 1);
                asmx->projectName[sizeof(asmx->projectName) - 1] = '\0';
            }

            ASMC *asmc = newAsmc(asmx);
            if (opt->detail) {
                printAsmc(asmc);
            }

            int r = build(asmc, opt->command == CMD_COMPILE ? ASMX_COMPILE : ASMX_BUILD);
            if (r != 0) {
                puts("Error: failed to build file");
                exit(1);
            }

            if (opt->command == CMD_RUN) {
                system(asmx->projectName);
            }

            freeAsmx(asmx);
            freeAsmc(asmc);
        }
            break;
        case CMD_CLEAN:
            remove_directory(BUILD_DIR);
            break;
        case CMD_HELP:
            print_help();
            break;
        case CMD_VERSION:
            print_version();
            break;
        case CMD_GEN:
            generate_default_config();
            break;
        default:
            printf("Unknown command. Use 'asmx -h' for usage information.\n");
            break;
    }

    free_options(opt);
    return 0;
}