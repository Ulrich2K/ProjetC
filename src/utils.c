#include <stdio.h>
#include <string.h>
#include "utils.h"

unsigned int simple_hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash += (unsigned int)(*str);
        str++;
    }
    return hash % 10000; // condensat sur 4 chiffres
}

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}
