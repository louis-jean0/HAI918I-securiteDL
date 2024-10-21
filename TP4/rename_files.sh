#!/bin/bash

# Dossier contenant les fichiers .ppm
source_dir="data/classes"

# Boucle pour traiter les fichiers se terminant par _0.ppm et _8.ppm
for file in "$source_dir"/*_{0,8}.ppm; do
    if [ -f "$file" ]; then  # Vérifier que le fichier existe
        # Extraire le chemin du fichier sans l'extension
        base_name=$(basename "$file" .ppm)
        
        # Renommer en fonction du suffixe
        if [[ "$base_name" == *"_0" ]]; then
            # Remplacer _0 par airplane
            new_name="${base_name/_0/airplane}.ppm"
        elif [[ "$base_name" == *"_8" ]]; then
            # Remplacer _8 par ship
            new_name="${base_name/_8/ship}.ppm"
        fi
        
        # Renommer le fichier
        mv "$file" "$source_dir/$new_name"
    fi
done

echo "Fichiers renommés avec succès."