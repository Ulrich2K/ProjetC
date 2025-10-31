# Makefile pour le projet de génération et recherche de condensats
# Nécessite: gcc, libssl-dev (OpenSSL)

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lssl -lcrypto

# Répertoires
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

# Fichiers sources et objets
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/generate.c $(SRC_DIR)/lookup.c $(SRC_DIR)/utils.c
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/generate.o $(OBJ_DIR)/lookup.o $(OBJ_DIR)/utils.o
EXECUTABLE = $(BIN_DIR)/condensat

# Règle par défaut
all: $(EXECUTABLE)

# Création du répertoire obj si nécessaire
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compilation de l'exécutable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✓ Compilation réussie: $(EXECUTABLE)"

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Dépendances des headers
$(OBJ_DIR)/main.o: $(SRC_DIR)/utils.h
$(OBJ_DIR)/generate.o: $(SRC_DIR)/utils.h
$(OBJ_DIR)/lookup.o: $(SRC_DIR)/utils.h
$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.h

# Nettoyage des fichiers compilés
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(EXECUTABLE)
	@echo "✓ Nettoyage effectué"

# Nettoyage complet (inclut les fichiers de test)
distclean: clean
	rm -f *.txt
	@echo "✓ Nettoyage complet effectué"

# Reconstruction complète
rebuild: clean all

# Aide
help:
	@echo "Makefile pour le projet condensat"
	@echo ""
	@echo "Commandes disponibles:"
	@echo "  make          - Compile le projet"
	@echo "  make clean    - Supprime les fichiers compilés"
	@echo "  make distclean- Nettoyage complet"
	@echo "  make rebuild  - Recompile tout le projet"
	@echo "  make help     - Affiche cette aide"
	@echo ""
	@echo "Prérequis:"
	@echo "  - gcc"
	@echo "  - libssl-dev (OpenSSL)"
	@echo ""
	@echo "Installation des prérequis sur Debian/Ubuntu:"
	@echo "  sudo apt-get install build-essential libssl-dev"

.PHONY: all clean distclean rebuild help