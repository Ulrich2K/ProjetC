# Programme de Génération et Recherche de Condensats

Programme en C permettant de générer des condensats cryptographiques (hash) à partir d'un dictionnaire et de rechercher les chaînes originales à partir de leurs condensats.

## Table des matières

- [Fonctionnalités](#fonctionnalités)
- [Prérequis](#prérequis)
- [Installation](#installation)
- [Compilation](#compilation)
- [Utilisation](#utilisation)
- [Algorithmes de hachage](#algorithmes-de-hachage)
- [Exemples](#exemples)
- [Structure du projet](#structure-du-projet)

## Fonctionnalités

- **Mode Génération (G)** : Génère une table de correspondance chaîne-condensat à partir d'un fichier dictionnaire
- **Mode Lookup (L)** : Recherche les chaînes originales à partir de condensats
- **Deux algorithmes de hachage** :
  - **Simple** : Hash basique (somme des caractères ASCII modulo 10000)
  - **SHA-256** : Algorithme cryptographique moderne et robuste

## Prérequis

### Système d'exploitation
- Linux (Ubuntu, Debian, ou distribution compatible)

### Logiciels requis
- `gcc` (compilateur C)
- `make` (outil de compilation)
- `libssl-dev` (bibliothèque OpenSSL pour SHA-256)

### Installation des prérequis sur Debian/Ubuntu

```bash
sudo apt-get update
sudo apt-get install build-essential libssl-dev
```

## Installation

1. Cloner le dépôt :
```bash
git clone <URL_DU_DEPOT>
cd <NOM_DU_DEPOT>
```

2. Vérifier que les fichiers sources sont dans le dossier `src/`

## Compilation

### Compilation standard

```bash
make
```

Cette commande compile tous les fichiers sources et crée l'exécutable `condensat` à la racine du projet.

### Autres commandes utiles

```bash
make clean      # Supprime les fichiers objets et l'exécutable
make distclean  # Nettoyage complet (inclut les fichiers de test)
make rebuild    # Recompile tout le projet
make help       # Affiche l'aide du Makefile
```

## Utilisation

### Mode Génération (G)

Génère une table de correspondance à partir d'un dictionnaire :

```bash
./condensat G <dictionnaire.txt> <table.txt> [algorithme]
```

**Paramètres :**
- `dictionnaire.txt` : fichier contenant les chaînes à hacher (une par ligne)
- `table.txt` : fichier de sortie pour la table de correspondance
- `algorithme` : `simple` ou `sha256` (optionnel, par défaut : `simple`)

### Mode Lookup (L)

Recherche des chaînes à partir de condensats :

```bash
./condensat L <table.txt>
```

**Paramètres :**
- `table.txt` : fichier contenant la table de correspondance

Le programme lit les condensats depuis l'entrée standard (stdin) et affiche les résultats sur la sortie standard (stdout).

Pour terminer la saisie : `Ctrl+D`

## Algorithmes de hachage

### Simple
- Hash basique calculé par somme des valeurs ASCII
- Résultat : entier entre 0 et 9999
- **Avantages** : Rapide, simple
- **Inconvénients** : Nombreuses collisions, non sécurisé

### SHA-256
- Algorithme cryptographique standard (famille SHA-2)
- Résultat : chaîne hexadécimale de 64 caractères
- **Avantages** : Très robuste, pratiquement aucune collision, sécurisé
- **Inconvénients** : Plus lent que le hash simple

## Exemples

### Exemple 1 : Génération avec SHA-256

```bash
# Créer un fichier dictionnaire
echo -e "password\nadmin\n123456\nqwerty" > dico.txt

# Générer la table avec SHA-256
./condensat G dico.txt table_sha256.txt sha256

# Résultat dans table_sha256.txt :
# Algorithm: SHA256
# password;5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8
# admin;8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918
# ...
```

### Exemple 2 : Recherche de condensats

```bash
# Rechercher des mots à partir de leurs condensats
./condensat L table_sha256.txt

# Saisir un condensat :
5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8

# Résultat affiché :
password
```

### Exemple 3 : Utilisation avec pipe

```bash
# Générer des condensats et les rechercher
echo "test" | sha256sum | cut -d' ' -f1 | ./condensat L table.txt
```

### Exemple 4 : Génération avec hash simple

```bash
./condensat G dico.txt table_simple.txt simple
```

## Structure du projet

```
.
README.md          # Ce fichier
Makefile           # Fichier de compilation
src/               # Code sou
    main.c         # Point d'entrée du programme
    generate.c     # Mode génération
    lookup.c       # Mode recherche
    utils.c        # Fonctions utilitaires et de hachage
    utils.h        # En-têtes
obj/               # Fichiers objets (créé lors de la compilation)
condensat          # Exécutable (créé lors de la compilation)
```

## Format des fichiers

### Fichier dictionnaire (entrée)
```
mot1
mot2
mot3
...
```

### Fichier table (sortie)
```
# Algorithm: SHA256
mot1;condensat1
mot2;condensat2
mot3;condensat3
...
```

## Performances

- **Hash simple** : ~1 million de hash/seconde
- **SHA-256** : ~100 000 hash/seconde (dépend du matériel)

La recherche est actuellement linéaire (O(n)). Pour de meilleures performances sur de grandes tables, envisager une structure de données optimisée (table de hachage, arbre, etc.).

## Limitations connues

- Longueur maximale des lignes : 256 caractères
- Recherche linéaire (non optimisée pour de très grandes tables)
- Le mode lookup charge la table depuis le fichier à chaque recherche

## Auteur

Projet réalisé dans le cadre d'un cours de programmation en C.

## Licence

Projet académique - Utilisation libre pour des fins éducatives.