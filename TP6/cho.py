import trimesh
import numpy as np

# Charger le modèle 3D
mesh = trimesh.load('bonfire.obj')
vertices = mesh.vertices

# Calcul du centre de gravité g
g = np.mean(vertices, axis=0)

# Centrer les sommets par rapport au centre de gravité
vertices_centered = vertices - g

# Conversion en coordonnées sphériques
def cartesian_to_spherical(x, y, z):
    rho = np.sqrt(x**2 + y**2 + z**2)
    # Éviter division par zéro
    rho[rho == 0] = 1e-8
    theta = np.arctan2(y, x)
    phi = np.arccos(z / rho)
    return rho, theta, phi

rho, theta, phi = cartesian_to_spherical(
    vertices_centered[:, 0], vertices_centered[:, 1], vertices_centered[:, 2]
)

# Définir le message à insérer
watermark_length = 10
watermark = np.random.choice([-1,1], size=watermark_length)  # Exemple
n_bins = len(watermark)

# Trier les indices des sommets selon rho
sorted_indices = np.argsort(rho)

# Diviser les indices triés en n_bins classes de taille égale
bins = np.array_split(sorted_indices, n_bins)

# Normalisation des bins entre [0, 1]
normalized_rho_bins = []

for bin_indices in bins:
    rho_bin = rho[bin_indices]
    min_rho = np.min(rho_bin)
    max_rho = np.max(rho_bin)
    range_rho = max_rho - min_rho
    if range_rho == 0:
        normalized_rho = np.zeros_like(rho_bin)
    else:
        normalized_rho = (rho_bin - min_rho) / range_rho
    normalized_rho_bins.append(normalized_rho)

# Insertion du message
alpha = 0.1
modified_rho_bins = []

for i, normalized_rho in enumerate(normalized_rho_bins):
    bit = watermark[i]
    adjusted_rho = normalized_rho.copy()
    iteration = 0
    max_iterations = 100

    while iteration < max_iterations:
        mean_rho = np.mean(adjusted_rho)
        if bit == 1:
            if mean_rho >= 0.5 + alpha:
                break
            else:
                delta = (0.5 + alpha) - mean_rho
                adjusted_rho += delta
        else:
            if mean_rho <= 0.5 - alpha:
                break
            else:
                delta = (0.5 - alpha) - mean_rho
                adjusted_rho += delta

        # S'assurer que les valeurs restent entre 0 et 1
        adjusted_rho = np.clip(adjusted_rho, 0.0, 1.0)
        iteration += 1

    modified_rho_bins.append(adjusted_rho)

# Remapping des valeurs ajustées
modified_rho = np.zeros_like(rho)

for bin_indices, adjusted_rho in zip(bins, modified_rho_bins):
    rho_bin = rho[bin_indices]
    min_rho = np.min(rho_bin)
    max_rho = np.max(rho_bin)
    range_rho = max_rho - min_rho
    if range_rho == 0:
        remapped_rho = np.full_like(adjusted_rho, min_rho)
    else:
        remapped_rho = adjusted_rho * range_rho + min_rho
    modified_rho[bin_indices] = remapped_rho

# Conversion en coordonnées cartésiennes
def spherical_to_cartesian(rho, theta, phi):
    x = rho * np.sin(phi) * np.cos(theta)
    y = rho * np.sin(phi) * np.sin(theta)
    z = rho * np.cos(phi)
    return x, y, z

x, y, z = spherical_to_cartesian(modified_rho, theta, phi)
modified_vertices_centered = np.vstack((x, y, z)).T

# Recentrer les sommets
modified_vertices = modified_vertices_centered + g

# Mise à jour du maillage
mesh.vertices = modified_vertices

# Sauvegarde du maillage tatoué
mesh.export('maillage_tatoue.obj')

# Vérification de l'intégrité du maillage
vertex_differences = np.linalg.norm(vertices - modified_vertices, axis=1)
print("Modification maximale d'un sommet :", np.max(vertex_differences))
print("Modification moyenne des sommets :", np.mean(vertex_differences))

# Extraction du message
# Fonction de conversion des coordonnées cartésiennes en sphériques
def cartesian_to_spherical(x, y, z):
    rho = np.sqrt(x**2 + y**2 + z**2)
    # Éviter division par zéro
    rho[rho == 0] = 1e-8
    theta = np.arctan2(y, x)
    phi = np.arccos(z / rho)
    return rho, theta, phi

# Charger le maillage tatoué
mesh_tatoue = trimesh.load('maillage_tatoue.obj')
vertices_tatoue = mesh_tatoue.vertices

# Recalculer le centre de gravité (doit être le même que celui utilisé lors de l'insertion)
g_tatoue = np.mean(vertices_tatoue, axis=0)

# Centrer les sommets par rapport au centre de gravité
vertices_tatoue_centered = vertices_tatoue - g_tatoue

# Conversion en coordonnées sphériques
rho_tatoue, theta_tatoue, phi_tatoue = cartesian_to_spherical(
    vertices_tatoue_centered[:, 0],
    vertices_tatoue_centered[:, 1],
    vertices_tatoue_centered[:, 2]
)

# Trier les indices des sommets selon rho
sorted_indices_tatoue = np.argsort(rho_tatoue)

# Nombre de bins (classes) utilisé lors de l'insertion
n_bins = len(watermark)

# Diviser les indices triés en n_bins classes de taille égale
bins_tatoue = np.array_split(sorted_indices_tatoue, n_bins)

# Normalisation des bins entre [0, 1]
normalized_rho_bins_tatoue = []

for bin_indices in bins_tatoue:
    rho_bin = rho_tatoue[bin_indices]
    min_rho = np.min(rho_bin)
    max_rho = np.max(rho_bin)
    range_rho = max_rho - min_rho
    if range_rho == 0:
        normalized_rho = np.zeros_like(rho_bin)
    else:
        normalized_rho = (rho_bin - min_rho) / range_rho
    normalized_rho_bins_tatoue.append(normalized_rho)

# Extraction du message
extracted_watermark = []

for normalized_rho in normalized_rho_bins_tatoue:
    mean_rho = np.mean(normalized_rho)
    if mean_rho > 0.5:
        extracted_watermark.append(1)
    else:
        extracted_watermark.append(-1)

print("Watermark extrait :", extracted_watermark)