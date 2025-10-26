/*
 * lookup.c - Implémentation de la recherche de condensats
 * 
 * Ce module implémente un système de recherche rapide utilisant une
 * table de hachage pour stocker et retrouver efficacement les paires
 * condensat-chaîne.
 * 
 * Complexité temporelle:
 *   - Insertion : O(1) en moyenne
 *   - Recherche : O(1) en moyenne
 *   - Chargement : O(n) où n est le nombre d'entrées
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lookup.h"

/* Constantes */
#define MAX_LINE_LENGTH 4096
#define HASH_TABLE_SIZE 1000003  /* Nombre premier pour meilleure distribution */
#define PROGRESS_INTERVAL 10000   /* Affichage progression tous les N */

/*
 * Structure pour une entrée de la table de hachage
 * 
 * Utilise le chaînage pour résoudre les collisions
 */
typedef struct HashEntry {
    char *hash;                  /* Condensat (clé) */
    char *value;                 /* Chaîne correspondante (valeur) */
    struct HashEntry *next;      /* Pointeur vers l'entrée suivante (chaînage) */
} HashEntry;

/* Table de hachage globale (tableau de pointeurs) */
static HashEntry *hash_table[HASH_TABLE_SIZE];

/*
 * Fonction de hachage djb2
 * 
 * Convertit une chaîne de caractères en un index de table.
 * Utilise l'algorithme djb2 de Dan Bernstein, connu pour sa bonne
 * distribution et sa simplicité.
 * 
 * Paramètres:
 *   str : Chaîne à hasher
 * 
 * Retour:
 *   Index dans la table de hachage [0, HASH_TABLE_SIZE-1]
 */
static unsigned int hash_function(const char *str) {
    unsigned long hash = 5381;
    int c;
    
    /* Algorithme djb2: hash = hash * 33 + c */
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash % HASH_TABLE_SIZE;
}

/*
 * Insère une paire hash-valeur dans la table de hachage
 * 
 * Gère automatiquement les collisions via chaînage.
 * 
 * Paramètres:
 *   hash  : Condensat (clé)
 *   value : Chaîne correspondante (valeur)
 * 
 * Retour:
 *   0  : Succès
 *   -1 : Erreur d'allocation mémoire
 */
static int insert_entry(const char *hash, const char *value) {
    unsigned int index = hash_function(hash);
    
    /* Allocation d'une nouvelle entrée */
    HashEntry *entry = malloc(sizeof(HashEntry));
    if (!entry) {
        return -1;
    }
    
    /* Copie des chaînes (duplication nécessaire) */
    entry->hash = strdup(hash);
    entry->value = strdup(value);
    
    /* Vérification des allocations */
    if (!entry->hash || !entry->value) {
        free(entry->hash);
        free(entry->value);
        free(entry);
        return -1;
    }
    
    /* Insertion en tête de liste (gestion des collisions par chaînage) */
    entry->next = hash_table[index];
    hash_table[index] = entry;
    
    return 0;
}

/*
 * Recherche une valeur à partir d'un condensat
 * 
 * Parcourt la liste chaînée à l'index calculé pour trouver l'entrée
 * correspondante.
 * 
 * Paramètres:
 *   hash : Condensat à rechercher
 * 
 * Retour:
 *   Pointeur vers la chaîne correspondante si trouvée
 *   NULL si non trouvée
 */
static const char* lookup_entry(const char *hash) {
    unsigned int index = hash_function(hash);
    HashEntry *entry = hash_table[index];
    
    /* Parcours de la liste chaînée à cet index */
    while (entry) {
        if (strcmp(entry->hash, hash) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return NULL;  /* Non trouvé */
}

/*
 * Libère toute la mémoire utilisée par la table de hachage
 * 
 * Parcourt toutes les entrées de la table et libère la mémoire
 * allouée pour chaque entrée et ses chaînes.
 */
static void free_hash_table(void) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashEntry *entry = hash_table[i];
        
        /* Libération de la liste chaînée à cet index */
        while (entry) {
            HashEntry *next = entry->next;
            free(entry->hash);
            free(entry->value);
            free(entry);
            entry = next;
        }
        
        hash_table[i] = NULL;
    }
}

/*
 * Charge le fichier T3C en mémoire dans la table de hachage
 * 
 * Lit le fichier T3C ligne par ligne et insère chaque paire
 * hash:valeur dans la table de hachage.
 * 
 * Paramètres:
 *   t3c_file : Chemin vers le fichier T3C
 * 
 * Retour:
 *   0  : Succès
 *   -1 : Erreur
 */
static int load_t3c_file(const char *t3c_file) {
    FILE *file = fopen(t3c_file, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier T3C");
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    int errors = 0;
    
    /* Initialisation de la table de hachage à NULL */
    memset(hash_table, 0, sizeof(hash_table));
    
    printf("Chargement de la table T3C...\n");
    
    /* Lecture ligne par ligne */
    while (fgets(line, sizeof(line), file)) {
        /* Suppression du saut de ligne '\n' */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        /* Recherche du séparateur ':' */
        char *colon = strchr(line, ':');
        if (!colon) {
            /* Format invalide, ligne ignorée */
            errors++;
            continue;
        }
        
        /* Séparation des deux parties */
        *colon = '\0';
        char *hash = line;
        char *value = colon + 1;
        
        /* Insertion dans la table de hachage */
        if (insert_entry(hash, value) != 0) {
            fprintf(stderr, "Erreur: Allocation mémoire impossible\n");
            fclose(file);
            free_hash_table();
            return -1;
        }
        
        count++;
        
        /* Affichage de la progression */
        if (count % PROGRESS_INTERVAL == 0) {
            printf("  Chargé: %d entrées\n", count);
            fflush(stdout);
        }
    }
    
    fclose(file);
    
    /* Affichage du résumé */
    printf("\nRésumé du chargement:\n");
    printf("  Entrées chargées avec succès : %d\n", count);
    if (errors > 0) {
        printf("  Lignes ignorées (format invalide) : %d\n", errors);
    }
    printf("\n");
    
    return 0;
}

/*
 * Fonction principale du mode lookup
 * 
 * Charge la table T3C, puis lit les condensats depuis stdin et
 * affiche les chaînes correspondantes sur stdout.
 * 
 * Paramètres:
 *   t3c_file : Chemin vers le fichier T3C
 * 
 * Retour:
 *   0  : Succès
 *   -1 : Erreur
 */
int lookup_hashes(const char *t3c_file) {
    /* Chargement de la table T3C en mémoire */
    if (load_t3c_file(t3c_file) != 0) {
        return -1;
    }
    
    printf("Prêt pour la recherche.\n");
    printf("Entrez les condensats (un par ligne).\n");
    printf("Appuyez sur Ctrl+D pour terminer.\n");
    printf("---\n");
    
    char hash_input[MAX_LINE_LENGTH];
    int found_count = 0;
    int not_found_count = 0;
    int total_searches = 0;
    
    /* Lecture des condensats depuis stdin */
    while (fgets(hash_input, sizeof(hash_input), stdin)) {
        /* Suppression du saut de ligne */
        size_t len = strlen(hash_input);
        if (len > 0 && hash_input[len - 1] == '\n') {
            hash_input[len - 1] = '\0';
        }
        
        /* Ignorer les lignes vides */
        if (strlen(hash_input) == 0) {
            continue;
        }
        
        total_searches++;
        
        /* Recherche dans la table */
        const char *result = lookup_entry(hash_input);
        
        if (result) {
            /* Condensat trouvé : affichage de la chaîne sur stdout */
            printf("%s\n", result);
            fflush(stdout);  /* Force l'affichage immédiat */
            found_count++;
        } else {
            /* Condensat non trouvé : pas d'affichage sur stdout */
            not_found_count++;
        }
    }
    
    /* Affichage des statistiques sur stderr */
    fprintf(stderr, "\n=== STATISTIQUES DE RECHERCHE ===\n");
    fprintf(stderr, "Total de recherches : %d\n", total_searches);
    fprintf(stderr, "Trouvés             : %d (%.1f%%)\n", 
            found_count, 
            total_searches > 0 ? (found_count * 100.0 / total_searches) : 0);
    fprintf(stderr, "Non trouvés         : %d (%.1f%%)\n", 
            not_found_count,
            total_searches > 0 ? (not_found_count * 100.0 / total_searches) : 0);
    
    /* Libération de la mémoire */
    free_hash_table();
    
    return 0;
}