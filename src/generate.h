/*
 * generate.h - Interface pour la génération de condensats cryptographiques
 * 
 * Ce module fournit les fonctions nécessaires pour générer des condensats
 * (hashes) à partir d'un fichier dictionnaire et créer une table de
 * correspondance T3C (Table de Correspondance Chaîne-Condensat).
 */

#ifndef GENERATE_H
#define GENERATE_H

/*
 * Génère une table T3C à partir d'un fichier dictionnaire
 * 
 * Cette fonction lit un fichier dictionnaire contenant des chaînes de
 * caractères (une par ligne, séparées par '\n'), calcule leur condensat
 * cryptographique et écrit le résultat dans un fichier T3C.
 * 
 * Paramètres:
 *   input_file  : Chemin vers le fichier dictionnaire d'entrée
 *                 Format attendu : une chaîne par ligne
 *   output_file : Chemin vers le fichier T3C de sortie
 *                 Format produit : hash:chaine (une entrée par ligne)
 *   algorithm   : Nom de l'algorithme de hachage à utiliser
 *                 Valeurs supportées : "SHA256"
 * 
 * Retour:
 *   0  : Succès
 *   -1 : Erreur (fichier inaccessible, algorithme non supporté, etc.)
 * 
 * Exemple:
 *   if (generate_t3c("dict.txt", "table.t3c", "SHA256") == 0) {
 *       printf("Génération réussie\n");
 *   }
 */
int generate_t3c(const char *input_file, const char *output_file, const char *algorithm);

#endif /* GENERATE_H */