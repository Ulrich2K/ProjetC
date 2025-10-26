#include <stdio.h>
#include <string.h>
#include "utils.h"

void mode_generate(const char *input_file, const char *output_file);
void mode_lookup(const char *table_file);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage : ./condensat [G|L] <fichiers>\n");
        return 1;
    }

    if (strcmp(argv[1], "G") == 0) {
        if (argc != 4) {
            printf("Usage : ./condensat G <dictionnaire.txt> <table.txt>\n");
            return 1;
        }
        mode_generate(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "L") == 0) {
        if (argc != 3) {
            printf("Usage : ./condensat L <table.txt>\n");
            return 1;
        }
        mode_lookup(argv[2]);
    }
    else {
        printf("Mode inconnu. Utilisez G (generate) ou L (lookup).\n");
        return 1;
    }

    return 0;
}