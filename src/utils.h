#ifndef UTILS_H
#define UTILS_H

#define MAX_LINE 256
#define SHA256_DIGEST_LENGTH 32
#define SHA256_HEX_LENGTH 65  // 64 caractères + '\0'

// Énumération des algorithmes de hachage disponibles
typedef enum {
    HASH_SIMPLE,    // Hash simple (original)
    HASH_SHA256     // SHA-256 (cryptographique)
} HashAlgorithm;

// Fonction de hash simple (originale)
unsigned int simple_hash(const char *str);

// Fonction de hachage SHA-256
// Paramètres:
//   - str: chaîne de caractères à hacher
//   - output: buffer de sortie (doit avoir au moins SHA256_HEX_LENGTH octets)
// Retourne: pointeur vers output
char* sha256_hash(const char *str, char *output);

// Fonction pour retirer le \n à la fin d'une chaîne
void remove_newline(char *str);

// Fonction pour convertir une chaîne hexadécimale en minuscules
void to_lowercase(char *str);

#endif
