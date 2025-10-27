#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void mode_lookup(const char *table_file) {
    FILE *ftable = fopen(table_file, "r");
    if (!ftable) {
        perror("Erreur ouverture fichier table");
        return;
    }

    printf("Entrez les condensats a rechercher (Ctrl+D pour terminer):\n");

    char input[MAX_LINE];
    char line[MAX_LINE];
    char word[MAX_LINE];
    unsigned int hash;

    while (fgets(input, sizeof(input), stdin)) {
        remove_newline(input);
        unsigned int target = atoi(input);
        int found = 0;

        rewind(ftable); // revenir au début du fichier
        while (fgets(line, sizeof(line), ftable)) {
            if (sscanf(line, "%[^;];%u", word, &hash) == 2) {
                if (hash == target) {
                    printf("%u correspond a %s \n", target, word);
                    found = 1;
                }
            }
        }
        if (!found)
            printf(" Aucun mot trouve pour le condensat %u\n", target);
    }

    fclose(ftable);
}

