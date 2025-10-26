#ifndef UTILS_H
#define UTILS_H

#define MAX_LINE 256

// Fonction de hash simple
unsigned int simple_hash(const char *str);

// Fonction pour retirer le \n à la fin d'une chaîne
void remove_newline(char *str);

#endif
