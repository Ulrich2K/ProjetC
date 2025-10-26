#!/bin/bash

# Script de tests pour HashTools
# Ce script effectue des tests de base pour vérifier le bon fonctionnement

set -e  # Arrêt en cas d'erreur

echo "==================================="
echo "Tests HashTools"
echo "==================================="
echo ""

# Couleurs pour l'affichage
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Variables
HASHTOOLS="../hashtools"
TEST_DICT="test_dict.txt"
TEST_T3C="test_output.t3c"
TEST_HASHES="test_hashes.txt"

# Vérification de l'existence de l'exécutable
if [ ! -f "$HASHTOOLS" ]; then
    echo -e "${RED}[ERREUR]${NC} L'exécutable hashtools n'existe pas"
    echo "Compilez d'abord avec 'make' à la racine du projet"
    exit 1
fi

# Test 1 : Génération de table T3C
echo "Test 1 : Génération de table T3C"
echo "---------------------------------"
$HASHTOOLS -m G -i $TEST_DICT -o $TEST_T3C
if [ -f "$TEST_T3C" ]; then
    echo -e "${GREEN}[OK]${NC} Fichier T3C créé"
    echo "Nombre d'entrées : $(wc -l < $TEST_T3C)"
else
    echo -e "${RED}[ERREUR]${NC} Fichier T3C non créé"
    exit 1
fi
echo ""

# Test 2 : Vérification du format T3C
echo "Test 2 : Vérification du format T3C"
echo "------------------------------------"
FIRST_LINE=$(head -n 1 $TEST_T3C)
if [[ $FIRST_LINE == *":"* ]]; then
    echo -e "${GREEN}[OK]${NC} Format correct (hash:chaine)"
    echo "Exemple : $FIRST_LINE"
else
    echo -e "${RED}[ERREUR]${NC} Format incorrect"
    exit 1
fi
echo ""

# Test 3 : Création de hashes de test
echo "Test 3 : Préparation des hashes de test"
echo "----------------------------------------"
cat > $TEST_HASHES << EOF
5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8
8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918
8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92
EOF
echo -e "${GREEN}[OK]${NC} Fichier de hashes créé"
echo ""

# Test 4 : Recherche avec lookup
echo "Test 4 : Recherche de hashes"
echo "-----------------------------"
RESULT=$($HASHTOOLS -m L -t $TEST_T3C < $TEST_HASHES 2>/dev/null)
if [ ! -z "$RESULT" ]; then
    echo -e "${GREEN}[OK]${NC} Recherche effectuée"
    echo "Résultats trouvés :"
    echo "$RESULT"
else
    echo -e "${RED}[ERREUR]${NC} Aucun résultat"
    exit 1
fi
echo ""

# Test 5 : Vérification d'un résultat spécifique
echo "Test 5 : Vérification hash 'password'"
echo "--------------------------------------"
PASS_HASH="5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8"
PASS_RESULT=$(echo "$PASS_HASH" | $HASHTOOLS -m L -t $TEST_T3C 2>/dev/null)
if [ "$PASS_RESULT" == "password" ]; then
    echo -e "${GREEN}[OK]${NC} Hash 'password' trouvé correctement"
else
    echo -e "${RED}[ERREUR]${NC} Résultat incorrect : $PASS_RESULT"
    exit 1
fi
echo ""

# Test 6 : Test hash inexistant
echo "Test 6 : Test hash inexistant"
echo "------------------------------"
FAKE_HASH="0000000000000000000000000000000000000000000000000000000000000000"
FAKE_RESULT=$(echo "$FAKE_HASH" | $HASHTOOLS -m L -t $TEST_T3C 2>/dev/null)
if [ -z "$FAKE_RESULT" ]; then
    echo -e "${GREEN}[OK]${NC} Hash inexistant correctement non trouvé"
else
    echo -e "${RED}[ERREUR]${NC} Résultat inattendu : $FAKE_RESULT"
fi
echo ""

# Nettoyage
echo "Nettoyage des fichiers de test..."
rm -f $TEST_T3C $TEST_HASHES
echo -e "${GREEN}[OK]${NC} Fichiers de test supprimés"
echo ""

echo "==================================="
echo -e "${GREEN}TOUS LES TESTS RÉUSSIS !${NC}"
echo "==================================="
