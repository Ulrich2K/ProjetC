/*
 * lookup.h - Interface pour la recherche de condensats
 * 
 * Ce module fournit les fonctions pour charger une table T3C en mémoire
 * et effectuer des recherches rapides de chaînes à partir de condensats.
 */

#ifndef LOOKUP_H
#define LOOKUP_H

/*
 * Charge une table T3C et effectue des recherches à partir de stdin
 * 
 * Cette fonction charge une table T3C en mémoire dans une structure
 * de données optimisée (table de hachage) puis lit des condensats
 * depuis l'entrée standard (stdin) et affiche les chaînes correspondantes
 * sur la sortie standard (stdout).
 * 
 * Paramètres:
 *   t3c_file : Chemin vers le fichier T3C à charger
 *              Format attendu : hash:chaine (une entrée par ligne)
 * 
 * Retour:
 *   0  : Succès
 *   -1 : Erreur (fichier inaccessible, mémoire insuffisante, etc.)
 * 
 * Fonctionnement:
 *   1. Charge la table T3C en mémoire dans une table de hachage
 *   2. Lit les condensats depuis stdin (un par ligne, séparés par '\n')
 *   3. Pour chaque condensat, recherche la chaîne correspondante
 *   4. Affiche les chaînes trouvées sur stdout
 *   5. Affiche les statistiques sur stderr
 * 
 * Exemple d'utilisation:
 *   echo "5e884898..." | ./hashtools -m L -t table.t3c
 *   ou
 *   ./hashtools -m L -t table.t3c < hashes.txt
 */
int lookup_hashes(const char *t3c_file);

#endif /* LOOKUP_H */