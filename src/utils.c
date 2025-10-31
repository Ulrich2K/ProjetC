#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <openssl/sha.h>
#include "utils.h"

/**
 * Fonction de hachage simple (originale)
 * Calcule la somme des valeurs ASCII des caractères modulo 10000
 * 
 * @param str: chaîne de caractères à hacher
 * @return: valeur de hachage sur 4 chiffres (0-9999)
 */
unsigned int simple_hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash += (unsigned int)(*str);
        str++;
    }
    return hash % 10000; // condensat sur 4 chiffres
}

/**
 * Fonction de hachage SHA-256
 * Génère un condensat cryptographique SHA-256 en format hexadécimal
 * 
 * @param str: chaîne de caractères à hacher
 * @param output: buffer pour stocker le résultat (64 caractères hex + '\0')
 * @return: pointeur vers output
 */
char* sha256_hash(const char *str, char *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    
    // Calcul du hash SHA-256
    SHA256((unsigned char*)str, strlen(str), hash);
    
    // Conversion en chaîne hexadécimale
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[SHA256_HEX_LENGTH - 1] = '\0';
    
    return output;
}

/**
 * Retire le caractère de nouvelle ligne '\n' à la fin d'une chaîne
 * 
 * @param str: chaîne de caractères à traiter
 */
void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

/**
 * Convertit une chaîne en minuscules
 * 
 * @param str: chaîne de caractères à convertir
 */
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}