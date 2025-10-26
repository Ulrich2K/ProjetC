/*
 * generate.c - Implémentation de la génération de condensats
 * 
 * Ce module implémente la génération de condensats cryptographiques SHA-256
 * à partir d'un fichier dictionnaire et la création d'une table T3C.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "generate.h"

/* Constantes */
#define MAX_LINE_LENGTH 4096        /* Taille maximale d'une ligne */
#define PROGRESS_INTERVAL 10000     /* Affichage progression tous les N mots */

/*
 * Calcule le hash SHA-256 d'une chaîne de caractères
 * 
 * Utilise la bibliothèque OpenSSL pour calculer le condensat SHA-256
 * et le convertit en représentation hexadécimale.
 * 
 * Paramètres:
 *   str    : Chaîne de caractères à hasher
 *   output : Buffer pour stocker le hash en hexadécimal
 *            Doit être alloué avec au moins 65 octets (64 hex + '\0')
 * 
 * Note: Cette fonction ne vérifie pas la taille du buffer output
 */
static void compute_sha256(const char *str, char *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];  /* 32 octets */
    SHA256_CTX sha256;
    
    /* Initialisation du contexte SHA-256 */
    SHA256_Init(&sha256);
    
    /* Mise à jour avec les données à hasher */
    SHA256_Update(&sha256, str, strlen(str));
    
    /* Finalisation et obtention du hash */
    SHA256_Final(hash, &sha256);
    
    /* Conversion du hash binaire en hexadécimal */
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';
}

/*
 * Génère une table T3C à partir d'un fichier dictionnaire
 * 
 * Lit le dictionnaire ligne par ligne, calcule le hash de chaque ligne
 * et écrit le résultat au format "hash:chaine" dans le fichier T3C.
 * 
 * Paramètres:
 *   input_file  : Chemin du fichier dictionnaire
 *   output_file : Chemin du fichier T3C de sortie
 *   algorithm   : Algorithme de hachage ("SHA256" uniquement pour l'instant)
 * 
 * Retour:
 *   0  : Succès
 *   -1 : Erreur
 */
int generate_t3c(const char *input_file, const char *output_file, const char *algorithm) {
    FILE *in = NULL;
    FILE *out = NULL;
    char line[MAX_LINE_LENGTH];
    char hash[65];  /* 64 caractères hex + '\0' */
    int count = 0;
    
    /* Vérification de l'algorithme demandé */
    if (strcmp(algorithm, "SHA256") != 0) {
        fprintf(stderr, "Erreur: Algorithme '%s' non supporté\n", algorithm);
        fprintf(stderr, "Algorithmes disponibles: SHA256\n");
        return -1;
    }
    
    /* Ouverture du fichier dictionnaire en lecture */
    in = fopen(input_file, "r");
    if (!in) {
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return -1;
    }
    
    /* Ouverture du fichier T3C en écriture */
    out = fopen(output_file, "w");
    if (!out) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        fclose(in);
        return -1;
    }
    
    printf("Début de la génération...\n");
    
    /* Lecture ligne par ligne du dictionnaire */
    while (fgets(line, sizeof(line), in)) {
        /* Suppression du caractère de fin de ligne '\n' */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        /* Ignorer les lignes vides */
        if (len == 0) {
            continue;
        }
        
        /* Calcul du condensat SHA-256 */
        compute_sha256(line, hash);
        
        /* Écriture dans le fichier T3C au format "hash:chaine" */
        fprintf(out, "%s:%s\n", hash, line);
        
        count++;
        
        /* Affichage de la progression tous les N mots */
        if (count % PROGRESS_INTERVAL == 0) {
            printf("  Traité: %d mots\n", count);
            fflush(stdout);  /* Force l'affichage immédiat */
        }
    }
    
    /* Fermeture des fichiers */
    fclose(in);
    fclose(out);
    
    /* Affichage du résultat final */
    printf("\nRésumé:\n");
    printf("  Total de condensats générés : %d\n", count);
    printf("  Fichier T3C créé : %s\n", output_file);
    
    return 0;
}