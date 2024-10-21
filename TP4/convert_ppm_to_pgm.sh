#!/bin/bash

# Dossier source contenant les fichiers .ppm
source_dir="data/classes"

# Dossier de destination pour les fichiers convertis (par exemple)
destination_dir="data/classes_converted"

# Créer le dossier de destination s'il n'existe pas
mkdir -p "$destination_dir"

# Boucle sur tous les fichiers .ppm dans le dossier source
for file in "$source_dir"/*.ppm; do
    # Vérifier que le fichier existe
    if [ -f "$file" ]; then
        # Extraire le nom de fichier sans extension
        filename=$(basename "$file" .ppm)

        # Chemin du fichier converti
        output_file="$destination_dir/${filename}.pgm"

        # Utiliser ImageMagick pour convertir le fichier
        magick "$file" "$output_file"

    fi
done

echo "Conversion terminée."