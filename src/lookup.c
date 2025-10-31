#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/**
 * Mode recherche (lookup) de condensats
 * Charge une table de correspondance (T3C) et recherche les chaînes
 * correspondant aux condensats fournis sur l'entrée standard
 * 
 * @param table_file: chemin du fichier T3C contenant les correspondances
 */
void mode_lookup(const char *table_file) {
    FILE *ftable = fopen(table_file, "r");
    if (!ftable) {
        perror("Erreur ouverture fichier table");
        return;
    }

    // Détection de l'algorithme utilisé dans la table
    HashAlgorithm algo = HASH_SIMPLE;
    char first_line[MAX_LINE];
    if (fgets(first_line, sizeof(first_line), ftable)) {
        if (strstr(first_line, "SHA256")) {
            algo = HASH_SHA256;
        }
        // Si ce n'est pas un commentaire, on revient au début
        if (first_line[0] != '#') {
            rewind(ftable);
        }
    }

    printf("Algorithme détecté: %s\n", algo == HASH_SHA256 ? "SHA-256" : "SIMPLE");
    printf("Entrez les condensats à rechercher (Ctrl+D pour terminer):\n");

    char input[MAX_LINE];
    char line[MAX_LINE];
    char word[MAX_LINE];
    char hash_str[SHA256_HEX_LENGTH];

    while (fgets(input, sizeof(input), stdin)) {
        remove_newline(input);
        
        // Ignorer les lignes vides
        if (strlen(input) == 0) continue;
        
        // Normaliser l'entrée en minuscules pour SHA-256
        if (algo == HASH_SHA256) {
            to_lowercase(input);
        }
        
        int found = 0;
        rewind(ftable); // Revenir au début du fichier
        
        // Sauter l'en-tête si présent
        if (algo == HASH_SHA256) {
            fgets(line, sizeof(line), ftable);
            if (line[0] != '#') {
                rewind(ftable);
            }
        }

        // Recherche dans la table
        while (fgets(line, sizeof(line), ftable)) {
            // Ignorer les commentaires
            if (line[0] == '#') continue;
            
            if (algo == HASH_SHA256) {
                // Format: mot;hash_sha256
                if (sscanf(line, "%[^;];%s", word, hash_str) == 2) {
                    to_lowercase(hash_str);
                    if (strcmp(hash_str, input) == 0) {
                        printf("%s\n", word);
                        found = 1;
                    }
                }
            } else {
                // Format: mot;hash_simple
                unsigned int hash;
                if (sscanf(line, "%[^;];%u", word, &hash) == 2) {
                    unsigned int target = atoi(input);
                    if (hash == target) {
                        printf("%s\n", word);
                        found = 1;
                    }
                }
            }
        }
        
        if (!found) {
            fprintf(stderr, "Aucun mot trouvé pour le condensat: %s\n", input);
        }
    }

    fclose(ftable);
}