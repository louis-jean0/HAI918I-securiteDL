import trimesh
import numpy as np
import matplotlib.pyplot as plt

# Charger le modèle 3D
mesh = trimesh.load('sphere.obj')
vertices = mesh.vertices

# Calcul du centre de gravité g
g = np.mean(vertices, axis=0)

# Centrer les sommets par rapport au centre de gravité
vertices_centered = vertices - g

# Conversion en coordonnées sphériques
def cartesian_to_spherical(x, y, z):
    rho = np.sqrt(x**2 + y**2 + z**2)
    rho[rho == 0] = 1e-8  # Éviter division par zéro
    theta = np.arctan2(y, x)
    phi = np.arccos(z / rho)
    return rho, theta, phi

rho, theta, phi = cartesian_to_spherical(
    vertices_centered[:, 0], vertices_centered[:, 1], vertices_centered[:, 2]
)

# Plage de valeurs pour le nombre de bins (taille du message)
bins_values = np.arange(5, 1005, 50)  # Varier le nombre de bins de 5 à 50
rmse_values = []

# Fixer alpha pour cette analyse
alpha = 0.1

for n_bins in bins_values:
    # Générer un watermark aléatoire de taille n_bins
    np.random.seed(42)  # Fixer la graine pour la reproductibilité
    watermark = np.random.choice([-1, 1], size=n_bins)

    # Copie de rho pour éviter de le modifier
    rho_modified = rho.copy()

    # Trier les indices des sommets selon rho
    sorted_indices = np.argsort(rho_modified)

    # Diviser les indices triés en n_bins classes de taille égale
    bins = np.array_split(sorted_indices, n_bins)

    # Normalisation des bins entre [0, 1]
    normalized_rho_bins = []
    for bin_indices in bins:
        rho_bin = rho_modified[bin_indices]
        min_rho = np.min(rho_bin)
        max_rho = np.max(rho_bin)
        range_rho = max_rho - min_rho
        if range_rho == 0:
            normalized_rho = np.zeros_like(rho_bin)
        else:
            normalized_rho = (rho_bin - min_rho) / range_rho
        normalized_rho_bins.append(normalized_rho)

    # Insertion du message
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
    rho_modified_final = rho_modified.copy()
    for bin_indices, adjusted_rho in zip(bins, modified_rho_bins):
        rho_bin = rho_modified[bin_indices]
        min_rho = np.min(rho_bin)
        max_rho = np.max(rho_bin)
        range_rho = max_rho - min_rho
        if range_rho == 0:
            remapped_rho = np.full_like(adjusted_rho, min_rho)
        else:
            remapped_rho = adjusted_rho * range_rho + min_rho
        rho_modified_final[bin_indices] = remapped_rho

    # Conversion en coordonnées cartésiennes
    def spherical_to_cartesian(rho, theta, phi):
        x = rho * np.sin(phi) * np.cos(theta)
        y = rho * np.sin(phi) * np.sin(theta)
        z = rho * np.cos(phi)
        return x, y, z

    x_mod, y_mod, z_mod = spherical_to_cartesian(rho_modified_final, theta, phi)
    modified_vertices_centered = np.vstack((x_mod, y_mod, z_mod)).T

    # Recentrer les sommets
    modified_vertices = modified_vertices_centered + g

    # Calcul du RMSE
    rmse = np.sqrt(np.mean((vertices - modified_vertices) ** 2))
    rmse_values.append(rmse)

# Tracé de la courbe nombre de bins vs RMSE
plt.figure(figsize=(8, 6))
plt.plot(bins_values, rmse_values, marker='o')
plt.title("Influence du nombre de bins sur le RMSE")
plt.xlabel("Nombre de bins (taille du message)")
plt.ylabel("RMSE")
plt.grid(True)
plt.show()