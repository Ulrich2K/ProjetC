# Image de base
FROM debian:bullseye-slim

# Installation des dépendances
RUN apt-get update && apt-get install -y \
    build-essential \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Création du répertoire de travail
WORKDIR /app

# Copie des fichiers sources
COPY src/ ./src/
COPY Makefile .

# Compilation
RUN make

# Point d'entrée
ENTRYPOINT ["./hashtools"]

# Commande par défaut (affiche l'aide)
CMD ["-h"]
