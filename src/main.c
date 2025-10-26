/*
 * main.c - Point d'entrée du programme HashTools
 * 
 * Ce programme permet de générer des condensats cryptographiques (SHA-256)
 * à partir d'un dictionnaire et de rechercher des chaînes de caractères
 * à partir de leurs condensats.
 * 
 * Modes disponibles :
 *   - G (Generate) : Génération de condensats
 *   - L (Lookup)   : Recherche de chaînes à partir de condensats
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "generate.h"
#include "lookup.h"

/* Affiche l'aide d'utilisation du programme */
void print_usage(const char *prog_name) {
    printf("HashTools - Générateur et recherche de condensats cryptographiques\n\n");
    printf("Usage: %s -m <mode> [options]\n\n", prog_name);
    
    printf("MODES:\n");
    printf("  G : Génération de condensats à partir d'un dictionnaire\n");
    printf("  L : Lookup/Recherche de chaînes à partir de condensats\n\n");
    
    printf("OPTIONS MODE G (Génération):\n");
    printf("  -i <fichier>  : Fichier dictionnaire d'entrée (obligatoire)\n");
    printf("                  Format : une chaîne par ligne séparée par \\n\n");
    printf("  -o <fichier>  : Fichier T3C de sortie (obligatoire)\n");
    printf("  -a <algo>     : Algorithme de hachage (optionnel, SHA256 par défaut)\n\n");
    
    printf("OPTIONS MODE L (Lookup):\n");
    printf("  -t <fichier>  : Fichier T3C à charger (obligatoire)\n");
    printf("                  Les condensats sont lus depuis stdin (un par ligne)\n\n");
    
    printf("OPTIONS GÉNÉRALES:\n");
    printf("  -h            : Affiche cette aide\n\n");
    
    printf("EXEMPLES:\n");
    printf("  # Génération d'une table T3C\n");
    printf("  %s -m G -i dictionnaire.txt -o table.t3c\n\n", prog_name);
    
    printf("  # Recherche avec pipe\n");
    printf("  echo \"5e884898...\" | %s -m L -t table.t3c\n\n", prog_name);
    
    printf("  # Recherche depuis un fichier\n");
    printf("  %s -m L -t table.t3c < hashes.txt\n\n", prog_name);
    
    printf("FORMAT FICHIER T3C:\n");
    printf("  hash:chaine (un par ligne)\n");
    printf("  Exemple: 5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8:password\n\n");
}

/*
 * Point d'entrée principal du programme
 */
int main(int argc, char *argv[]) {
    int opt;
    char mode = 0;
    char *input_file = NULL;
    char *output_file = NULL;
    char *t3c_file = NULL;
    char *algorithm = "SHA256";
    
    /* Vérification du nombre d'arguments minimal */
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    /* Parsing des arguments de ligne de commande avec getopt */
    while ((opt = getopt(argc, argv, "m:i:o:t:a:h")) != -1) {
        switch (opt) {
            case 'm':
                /* Mode de fonctionnement : G (generate) ou L (lookup) */
                mode = optarg[0];
                break;
            
            case 'i':
                /* Fichier d'entrée (dictionnaire pour mode G) */
                input_file = optarg;
                break;
            
            case 'o':
                /* Fichier de sortie (T3C pour mode G) */
                output_file = optarg;
                break;
            
            case 't':
                /* Fichier T3C pour le mode lookup (L) */
                t3c_file = optarg;
                break;
            
            case 'a':
                /* Algorithme de hachage (extensible) */
                algorithm = optarg;
                break;
            
            case 'h':
                /* Affichage de l'aide */
                print_usage(argv[0]);
                return 0;
            
            default:
                /* Option invalide */
                fprintf(stderr, "Option invalide\n\n");
                print_usage(argv[0]);
                return 1;
        }
    }
    
    /* Vérification du mode sélectionné */
    if (mode != 'G' && mode != 'L') {
        fprintf(stderr, "Erreur: Mode invalide ou non spécifié.\n");
        fprintf(stderr, "Utilisez -m G (génération) ou -m L (lookup)\n\n");
        print_usage(argv[0]);
        return 1;
    }
    
    /* Traitement selon le mode sélectionné */
    if (mode == 'G') {
        /* ===== MODE GÉNÉRATION ===== */
        
        /* Vérification des paramètres obligatoires */
        if (!input_file || !output_file) {
            fprintf(stderr, "Erreur: Le mode G nécessite -i (entrée) et -o (sortie)\n\n");
            print_usage(argv[0]);
            return 1;
        }
        
        /* Affichage des paramètres */
        printf("=== MODE GÉNÉRATION ===\n");
        printf("Fichier dictionnaire : %s\n", input_file);
        printf("Fichier T3C sortie   : %s\n", output_file);
        printf("Algorithme           : %s\n", algorithm);
        printf("\n");
        
        /* Appel de la fonction de génération */
        if (generate_t3c(input_file, output_file, algorithm) != 0) {
            fprintf(stderr, "\nErreur: La génération a échoué\n");
            return 1;
        }
        
        printf("\n=== GÉNÉRATION TERMINÉE AVEC SUCCÈS ===\n");
        
    } else if (mode == 'L') {
        /* ===== MODE LOOKUP ===== */
        
        /* Vérification des paramètres obligatoires */
        if (!t3c_file) {
            fprintf(stderr, "Erreur: Le mode L nécessite -t (fichier T3C)\n\n");
            print_usage(argv[0]);
            return 1;
        }
        
        /* Affichage des paramètres */
        printf("=== MODE LOOKUP ===\n");
        printf("Fichier T3C : %s\n", t3c_file);
        printf("\n");
        
        /* Appel de la fonction de lookup */
        if (lookup_hashes(t3c_file) != 0) {
            fprintf(stderr, "\nErreur: Le lookup a échoué\n");
            return 1;
        }
    }
    
    return 0;
}