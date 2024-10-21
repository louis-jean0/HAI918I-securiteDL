# Désactiver les suffixes et les règles implicites
.SUFFIXES:

# Compilateur C++
CC = g++

# Options de compilation
CFLAGS = -Wall -g

# Fichiers sources pour les modes de chiffrement et AES
SOURCES = $(wildcard *.cpp)
AES_SRC = AES.cpp
AES_OBJ = AES.o

# Exclure AES.cpp de SOURCES pour éviter de le compiler plusieurs fois
SOURCES := $(filter-out $(AES_SRC), $(SOURCES))

# Obtenir la liste des exécutables (en retirant l'extension .cpp)
EXECUTABLES := $(SOURCES:.cpp=)

# Règle par défaut
all: $(EXECUTABLES)

# Compilation de chaque exécutable avec la bibliothèque AES liée
%: %.o $(AES_OBJ)
	$(CC) $(CFLAGS) $< $(AES_OBJ) -o $@

# Compilation d'AES.cpp en AES.o
$(AES_OBJ): $(AES_SRC)
	$(CC) $(CFLAGS) -c $(AES_SRC) -o $(AES_OBJ)

# Compilation des fichiers .cpp en .o
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyer les fichiers compilés
clean:
	rm -f *.o $(EXECUTABLES)