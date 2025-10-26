# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -std=c99
LDFLAGS = -lssl -lcrypto

# Nom de l'exécutable
TARGET = hashtools

# Répertoires
SRCDIR = src
OBJDIR = obj

# Fichiers sources et objets
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/generate.c $(SRCDIR)/lookup.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Règle par défaut
all: $(TARGET)

# Création du répertoire obj si nécessaire
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Compilation de l'exécutable
$(TARGET): $(OBJDIR) $(OBJECTS)
	@echo "Édition des liens..."
	@$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilation terminée : $(TARGET)"

# Compilation des fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compilation de $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	@echo "Nettoyage..."
	@rm -rf $(OBJDIR) $(TARGET)
	@echo "Nettoyage terminé"

# Recompilation complète
re: clean all

# Tests
test: $(TARGET)
	@echo "Exécution des tests..."
	@cd tests && ./run_tests.sh

# Aide
help:
	@echo "Makefile pour HashTools"
	@echo ""
	@echo "Cibles disponibles :"
	@echo "  all (défaut) : Compile le programme"
	@echo "  clean        : Supprime les fichiers générés"
	@echo "  re           : Recompile complètement"
	@echo "  test         : Exécute les tests"
	@echo "  help         : Affiche cette aide"

.PHONY: all clean re test help