# Compilateur C++
CC = g++

# Options de compilation
CFLAGS = -Wall -g

# Trouver tous les fichiers .cpp dans le dossier actuel
SOURCES = $(wildcard *.cpp)

# Convertir les noms de fichiers .cpp en noms d'exécutables
EXECUTABLES = $(SOURCES:%.cpp=%)

# Règle par défaut
all: $(EXECUTABLES)

# Règle pour compiler chaque fichier source en un exécutable
%: %.cpp
	$(CC) $(CFLAGS) $< -o $@

# Nettoyer les fichiers compilés
clean:
	rm -f $(EXECUTABLES)