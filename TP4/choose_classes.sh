#!/bin/bash

# Dossier source contenant les fichiers .ppm
source_dir="data/images"

# Dossier de destination
destination_dir="data/classes"

# Créer le dossier de destination s'il n'existe pas
mkdir -p "$destination_dir"

# Boucle pour trouver et déplacer les fichiers se terminant par _0.ppm et _8.ppm
for file in "$source_dir"/*_{0,8}.ppm; do
    if [ -f "$file" ]; then  # Vérifier que le fichier existe
        cp "$file" "$destination_dir"  # Déplacer le fichier
    fi
done

echo "Fichiers _0.ppm et _8.ppm déplacés dans $destination_dir."