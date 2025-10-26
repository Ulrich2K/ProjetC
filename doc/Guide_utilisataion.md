# Guide d'Utilisation Détaillé - HashTools

## Table des matières

1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Utilisation basique](#utilisation-basique)
4. [Cas d'usage avancés](#cas-dusage-avancés)
5. [Performance](#performance)
6. [Dépannage](#dépannage)

## Introduction

HashTools est un outil de génération et de recherche de condensats cryptographiques (hashes). Il est particulièrement utile pour :

- Générer des rainbow tables
- Effectuer des recherches inversées de hashes
- Analyser la sécurité de mots de passe
- Étudier les algorithmes de hachage

## Installation

### Prérequis
```bash
# Debian/Ubuntu
sudo apt-get install build-essential libssl-dev

# Fedora
sudo dnf install gcc openssl-devel make

# Arch Linux
sudo pacman -S base-devel openssl
```

### Compilation
```bash
git clone <votre-repo>
cd hashtools-project
make
```

### Vérification
```bash
./hashtools -h
```

## Utilisation basique

### Créer votre premier dictionnaire
```bash
cat > mon_dict.txt << EOF
password
123456
qwerty
admin
letmein
monkey
dragon
master
welcome
login
EOF
```

### Générer la table T3C
```bash
./hashtools -m G -i mon_dict.txt -o ma_table.t3c
```

### Tester la recherche
```bash
# Le hash de "password" est :
# 5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8

echo "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8" | \
  ./hashtools -m L -t ma_table.t3c
```

Résultat attendu : `password`

## Cas d'usage avancés

### 1. Traitement de gros dictionnaires
```bash
# Avec un dictionnaire de 1 million de mots
./hashtools -m G -i rockyou.txt -o rockyou.t3c

# Le chargement prendra quelques secondes
# La recherche sera quasi-instantanée
```

### 2. Recherche en masse
```bash
# Créer un fichier avec plusieurs hashes
cat > hashes_to_find.txt << EOF
5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8
8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918
e10adc3949ba59abbe56e057f20f883e
EOF

# Rechercher tous les hashes
./hashtools -m L -t ma_table.t3c < hashes_to_find.txt
```

### 3. Pipeline avec autres outils
```bash
# Extraire des hashes d'un fichier et les rechercher
grep -oE '[a-f0-9]{64}' fichier_logs.txt | \
  ./hashtools -m L -t ma_table.t3c

# Combiner avec sort et uniq
cat hashes.txt | sort | uniq | \
  ./hashtools -m L -t ma_table.t3c
```

### 4. Générer des variations
```bash
# Créer un dictionnaire avec variations
cat base_dict.txt | while read word; do
  echo "$word"
  echo "${word}123"
  echo "${word}!"
  echo "${word}2024"
done > dict_variations.txt

./hashtools -m G -i dict_variations.txt -o variations.t3c
```

### 5. Statistiques et analyse
```bash
# Compter les entrées dans une table T3C
wc -l ma_table.t3c

# Voir les premiers résultats
head -n 10 ma_table.t3c

# Rechercher un mot spécifique dans la table
grep ":password$" ma_table.t3c
```

## Performance

### Temps de génération

| Taille dictionnaire | Temps approx. | Taille T3C |
|---------------------|---------------|------------|
| 1 000 mots         | < 1 seconde   | ~100 KB    |
| 10 000 mots        | 1-2 secondes  | ~1 MB      |
| 100 000 mots       | 10-20 secondes| ~10 MB     |
| 1 000 000 mots     | 2-3 minutes   | ~100 MB    |

### Temps de recherche

La recherche est **quasi-instantanée** quelle que soit la taille de la table grâce à la table de hachage (O(1)).

### Consommation mémoire

La mémoire utilisée en mode lookup est proportionnelle à la taille du dictionnaire :
- ~100 octets par entrée en moyenne
- Exemple : 1 million d'entrées ≈ 100 MB de RAM

## Dépannage

### Erreur : "libssl.so not found"
```bash
# Debian/Ubuntu
sudo apt-get install libssl-dev

# Vérifier l'installation
ldconfig -p | grep ssl
```

### Erreur : "Permission denied"
```bash
chmod +x hashtools
```

### Fichier T3C corrompu

Vérifiez le format :
```bash
head ma_table.t3c
# Chaque ligne doit être : hash:chaine
```

### Performance dégradée

- Vérifiez que vous utilisez un SSD
- Augmentez la RAM disponible
- Utilisez des dictionnaires triés

### Compilation échoue
```bash
# Nettoyez et recompilez
make clean
make

# Vérifiez GCC
gcc --version
```

## Exemples de hashes courants

Pour tester rapidement :

| Mot de passe | SHA-256 |
|--------------|---------|
| password | 5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8 |
| admin | 8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918 |
| 123456 | 8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92 |
| qwerty | 65e84be33532fb784c48129675f9eff3a682b27168c0ea744b2cf58ee02337c5 |

## Conseils de sécurité

⚠️ **Important** :
- N'utilisez jamais cet outil pour des activités illégales
- Les rainbow tables ne cassent que les mots de passe faibles
- Utilisez toujours des mots de passe forts et uniques
- Utilisez un gestionnaire de mots de passe

## Support

Pour toute question ou problème :
1. Consultez le README.md
2. Vérifiez les issues sur le dépôt
3. Créez une nouvelle issue si nécessaire