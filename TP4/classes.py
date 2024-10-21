import os
import random
import shutil

# Chemin vers le dossier contenant toutes les images
dossier_source = 'data/classes_converted'

# Chemin vers le nouveau dossier où les images sélectionnées seront copiées
dossier_destination = 'data/dest'

# Créer le dossier de destination s'il n'existe pas
if not os.path.exists(dossier_destination):
    os.makedirs(dossier_destination)

# Liste des classes
classes = ['ship', 'airplane']

# Pour chaque classe, sélectionner 100 images aléatoires et les copier
for classe in classes:
    # Obtenir la liste des fichiers correspondant à la classe
    images_classe = [f for f in os.listdir(dossier_source) if classe in f]

    # Vérifier qu'il y a au moins 100 images pour la classe
    if len(images_classe) < 100:
        print(f"Il n'y a que {len(images_classe)} images pour la classe '{classe}'.")
        continue

    # Sélectionner aléatoirement 100 images
    images_selectionnees = random.sample(images_classe, 100)

    # Copier les images sélectionnées dans le dossier de destination
    for image in images_selectionnees:
        chemin_source = os.path.join(dossier_source, image)
        chemin_destination = os.path.join(dossier_destination, image)
        shutil.copyfile(chemin_source, chemin_destination)

    print(f"100 images de la classe '{classe}' ont été copiées dans '{dossier_destination}'.")

print("Opération terminée.")