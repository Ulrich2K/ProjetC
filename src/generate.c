#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void mode_generate(const char *input_file, const char *output_file) {
    FILE *fin = fopen(input_file, "r");
    if (!fin) {
        perror("Erreur ouverture fichier dictionnaire");
        return;
    }

    FILE *fout = fopen(output_file, "w");
    if (!fout) {
        perror("Erreur ouverture fichier sortie");
        fclose(fin);
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fin)) {
        remove_newline(line);
        if (strlen(line) == 0) continue;

        unsigned int hash = simple_hash(line);
        fprintf(fout, "%s;%u\n", line, hash);
    }

    printf("Table de correspondance genereée dans '%s'\n", output_file);

    fclose(fin);
    fclose(fout);
}
