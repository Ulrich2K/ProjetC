
## Description

Programme en C permettant de générer des condensats cryptographiques (hashes SHA-256) à partir d'un fichier dictionnaire et de rechercher rapidement des chaînes de caractères à partir de leurs condensats.

## Pré-requis

### Environnement Linux
- Système d'exploitation : Linux (Debian/Ubuntu recommandé)
- Compilateur : GCC
- Bibliothèque : OpenSSL

### Installation des dépendances
```bash
# Debian/Ubuntu
sudo apt-get update
sudo apt-get install build-essential libssl-dev

### Alternative avec Docker
Si vous préférez utiliser Docker (voir section Docker ci-dessous).

## Compilation

### Compilation standard
```bash
make
```

Le programme compilé s'appellera `hashtools`.

### Nettoyage
```bash
make clean    # Supprime les fichiers objets et l'exécutable
make re       # Recompile complètement
```

## Utilisation

### Mode G - Génération de condensats

Génère une table de correspondance chaîne-condensat (T3C) à partir d'un fichier dictionnaire.

**Syntaxe :**
```bash
./hashtools -m G -i <fichier_dictionnaire> -o <fichier_t3c> [-a <algorithme>]
```

**Paramètres :**
- `-m G` : Mode génération (obligatoire)
- `-i <fichier>` : Fichier dictionnaire d'entrée, un mot par ligne (obligatoire)
- `-o <fichier>` : Fichier T3C de sortie (obligatoire)
- `-a <algo>` : Algorithme de hachage (optionnel, SHA256 par défaut)

**Exemple :**
```bash
./hashtools -m G -i dictionnaire.txt -o table.t3c
```

**Format du fichier dictionnaire :**
```
password
admin
123456
letmein
```

### Mode L - Lookup/Recherche

Recherche des chaînes de caractères à partir de leurs condensats.

**Syntaxe :**
```bash
./hashtools -m L -t <fichier_t3c>
```

**Paramètres :**
- `-m L` : Mode lookup (obligatoire)
- `-t <fichier>` : Fichier T3C à charger (obligatoire)
- Les condensats sont lus depuis l'entrée standard (stdin)

**Exemple avec pipe :**
```bash
echo "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8" | ./hashtools -m L -t table.t3c
```

**Exemple avec fichier :**
```bash
./hashtools -m L -t table.t3c < hashes.txt
```

**Exemple interactif :**
```bash
./hashtools -m L -t table.t3c
# Entrez les hashes un par ligne
# Appuyez sur Ctrl+D pour terminer
```

## Exemples complets

### Exemple 1 : Création et utilisation basique
```bash
# 1. Créer un fichier dictionnaire
cat > dict.txt << EOF
password
admin
123456
qwerty
letmein
EOF

# 2. Générer la table T3C
./hashtools -m G -i dict.txt -o table.t3c

# 3. Rechercher un hash
echo "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8" | ./hashtools -m L -t table.t3c
# Affiche : password
```

### Exemple 2 : Recherche multiple
```bash
# Créer un fichier avec plusieurs hashes
cat > hashes.txt << EOF
5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8
8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918
e10adc3949ba59abbe56e057f20f883e
EOF

# Rechercher tous les hashes
./hashtools -m L -t table.t3c < hashes.txt
```

## Format du fichier T3C

Le fichier T3C (Table de Correspondance Chaîne-Condensat) est un fichier texte avec une entrée par ligne au format :
```
<hash_hexadecimal>:<chaine_originale>
```

**Exemple :**
```
5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8:password
8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918:admin
```

## Structure du code
```
src/
├── main.c        # Point d'entrée, parsing des arguments
├── generate.h    # Interface pour la génération
├── generate.c    # Implémentation génération (SHA-256)
├── lookup.h      # Interface pour la recherche
└── lookup.c      # Implémentation recherche (table de hachage)
```

## Performance

Le programme utilise une table de hachage pour garantir des recherches rapides :
- **Génération** : ~100 000 hashes/seconde (dépend du CPU)

- **Recherche** : O(1) en moyenne, très rapide même avec des millions d'entrées
