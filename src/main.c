#include <stdio.h>
#include <string.h>
#include "utils.h"

void mode_generate(const char *input_file, const char *output_file, HashAlgorithm algo);
void mode_lookup(const char *table_file);

/**
 * Affiche l'aide d'utilisation du programme
 */
void print_usage() {
    printf("=== Programme de génération et recherche de condensats ===\n\n");
    printf("Usage:\n");
    printf("  Mode Génération:\n");
    printf("    ./condensat G <dictionnaire.txt> <table.txt> [algorithme]\n");
    printf("      algorithme: simple (par défaut) ou sha256\n\n");
    printf("  Mode Lookup:\n");
    printf("    ./condensat L <table.txt>\n\n");
    printf("Exemples:\n");
    printf("  ./condensat G dico.txt table.txt sha256\n");
    printf("  ./condensat G dico.txt table.txt simple\n");
    printf("  ./condensat L table.txt\n");
}

int main(int argc, char *argv[]) {
    // Vérification du nombre minimum d'arguments
    if (argc < 2) {
        print_usage();
        return 1;
    }

    // Mode Génération
    if (strcmp(argv[1], "G") == 0) {
        if (argc < 4) {
            printf("Erreur: arguments manquants pour le mode Génération\n\n");
            print_usage();
            return 1;
        }
        
        // Détermination de l'algorithme (par défaut: simple)
        HashAlgorithm algo = HASH_SIMPLE;
        if (argc >= 5) {
            if (strcmp(argv[4], "sha256") == 0 || strcmp(argv[4], "SHA256") == 0) {
                algo = HASH_SHA256;
            } else if (strcmp(argv[4], "simple") == 0 || strcmp(argv[4], "SIMPLE") == 0) {
                algo = HASH_SIMPLE;
            } else {
                printf("Algorithme inconnu: %s\n", argv[4]);
                printf("Algorithmes disponibles: simple, sha256\n");
                return 1;
            }
        }
        
        printf("Démarrage du mode Génération...\n");
        mode_generate(argv[2], argv[3], algo);
    }
    // Mode Lookup
    else if (strcmp(argv[1], "L") == 0) {
        if (argc != 3) {
            printf("Erreur: arguments manquants pour le mode Lookup\n\n");
            print_usage();
            return 1;
        }
        
        printf("Démarrage du mode Lookup...\n");
        mode_lookup(argv[2]);
    }
    // Mode inconnu
    else {
        printf("Mode inconnu: %s\n\n", argv[1]);
        print_usage();
        return 1;
    }

    return 0;
}