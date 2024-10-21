import trimesh
import numpy as np
import matplotlib.pyplot as plt

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

# Définir le message à insérer (fixe pour toutes les valeurs de alpha)
np.random.seed(42)  # Fixer la graine pour la reproductibilité
watermark_length = 10
watermark = np.random.choice([-1, 1], size=watermark_length)
n_bins = len(watermark)

# Plage de valeurs pour alpha
alpha_values = np.linspace(0.05, 1, 20)  # 20 valeurs entre 0.05 et 0.5
rmse_values = []

for alpha in alpha_values:
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

    # Vous pouvez également enregistrer le maillage pour certaines valeurs de alpha si vous le souhaitez
    # Par exemple, pour alpha = 0.1
    # if np.isclose(alpha, 0.1):
    #     mesh.vertices = modified_vertices
    #     mesh.export(f'maillage_tatoue_alpha_{alpha:.2f}.obj')

# Tracé de la courbe alpha vs RMSE
plt.figure(figsize=(8, 6))
plt.plot(alpha_values, rmse_values, marker='o')
plt.title(r"Influence de $\alpha$ sur le RMSE")
plt.xlabel(r"$\alpha$ (force d'insertion)")
plt.ylabel("RMSE")
plt.grid(True)
plt.show()