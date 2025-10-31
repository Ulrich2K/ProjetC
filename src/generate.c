#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/**
 * Mode génération de condensats
 * Lit un fichier dictionnaire et génère une table de correspondance
 * chaîne-condensat (T3C) dans un fichier de sortie
 * 
 * @param input_file: chemin du fichier dictionnaire (une chaîne par ligne)
 * @param output_file: chemin du fichier de sortie pour la T3C
 * @param algo: algorithme de hachage à utiliser (HASH_SIMPLE ou HASH_SHA256)
 */
void mode_generate(const char *input_file, const char *output_file, HashAlgorithm algo) {
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

    // Écriture de l'en-tête du fichier T3C pour indiquer l'algorithme utilisé
    fprintf(fout, "# Algorithm: %s\n", algo == HASH_SHA256 ? "SHA256" : "SIMPLE");

    char line[MAX_LINE];
    int count = 0;
    
    while (fgets(line, sizeof(line), fin)) {
        remove_newline(line);
        if (strlen(line) == 0) continue;

        if (algo == HASH_SHA256) {
            // Utilisation de SHA-256
            char hash_output[SHA256_HEX_LENGTH];
            sha256_hash(line, hash_output);
            fprintf(fout, "%s;%s\n", line, hash_output);
        } else {
            // Utilisation du hash simple
            unsigned int hash = simple_hash(line);
            fprintf(fout, "%s;%u\n", line, hash);
        }
        
        count++;
    }

    printf("Table de correspondance générée dans '%s'\n", output_file);
    printf("Algorithme utilisé: %s\n", algo == HASH_SHA256 ? "SHA-256" : "SIMPLE");
    printf("Nombre d'entrées: %d\n", count);

    fclose(fin);
    fclose(fout);
}