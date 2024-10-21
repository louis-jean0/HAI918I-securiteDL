#include "image_ppm.h"
#include <iostream>
#include <random>
#include <filesystem>
#include <vector>
#include <cstring>
#include <cmath>

namespace fs = std::filesystem;

// Filtres première couche (inchangés)
short fpb_1[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
short fpb_2[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
short fph_1[9] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
short fph_2[9] = {-1, -2, -1, -1, 11, -1, -1, -2, -1};
short f_random[9];

// Filtres deuxième couche (inchangés)
short filtre2_fpb[9] = {1, 0, -1, 1, 0, -1, 1, 0, -1};
short filtre2_fph[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
short filtre2_random[9];

void initialiser_filtres_random() {
    srand(time(NULL));
    for(size_t i = 0; i < 9; ++i) {
        f_random[i] = (rand() % 21) - 10;
        filtre2_random[i] = (rand() % 21) - 10;
    }
}

int traiter_image(const std::string& chemin_image, float W[2][540], OCTET* vecteur_caracteristiques);

float calculer_PSNR(OCTET* imagette_test, float* imagette_moyenne, int taille);

void convertir_imagette_moyenne_en_OCTET(float* imagette_moyenne, OCTET* imagette_OCTET, int taille);

int main(int argc, char* argv[]) {

    initialiser_filtres_random();

    // Initialisation des poids
    float W[2][540];
    srand(time(NULL));
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 540; ++j) {
            W[i][j] = ((float)rand() / RAND_MAX) * 0.01f;
        }   
    }

    // Dossiers d'entraînement et de test
    std::string train_dirs[2] = {"data/train/class0", "data/train/class1"};
    std::string test_dirs[2] = {"data/test/class0", "data/test/class1"};

    // Imagettes moyennes
    float imagette_moyenne_class0[540] = {0};
    float imagette_moyenne_class1[540] = {0};
    int count_class0 = 0;
    int count_class1 = 0;

    // === Phase d'entraînement ===
    for(int classe = 0; classe < 2; ++classe) {
        for(const auto& entry : fs::directory_iterator(train_dirs[classe])) {
            std::string chemin_image = entry.path().string();

            OCTET* vecteur_caracteristiques = new OCTET[540];

            int classe_predite = traiter_image(chemin_image, W, vecteur_caracteristiques);

            // Accumuler les vecteurs pour calculer l'imagette moyenne
            if(classe == 0) {
                for(int i = 0; i < 540; ++i) {
                    imagette_moyenne_class0[i] += vecteur_caracteristiques[i];
                }
                count_class0++;
            } else {
                for(int i = 0; i < 540; ++i) {
                    imagette_moyenne_class1[i] += vecteur_caracteristiques[i];
                }
                count_class1++;
            }

            delete[] vecteur_caracteristiques; // Libération de la mémoire
        }
    }

    // Calcul des imagettes moyennes
    for(int i = 0; i < 540; ++i) {
        if(count_class0 > 0) {
            imagette_moyenne_class0[i] /= count_class0;
        }
        if(count_class1 > 0) {
            imagette_moyenne_class1[i] /= count_class1;
        }
    }

    OCTET imagette_OCTET_class0[540];
    convertir_imagette_moyenne_en_OCTET(imagette_moyenne_class0, imagette_OCTET_class0, 540);
    ecrire_image_pgm("imagette_moyenne_class0.pgm", imagette_OCTET_class0, 27, 20);

    OCTET imagette_OCTET_class1[540];
    convertir_imagette_moyenne_en_OCTET(imagette_moyenne_class1, imagette_OCTET_class1, 540);
    ecrire_image_pgm("imagette_moyenne_class1.pgm", imagette_OCTET_class1, 27, 20);


    // === Phase de test ===

    int TP = 0, FP = 0, TN = 0, FN = 0;

    float psnr_sum_class0 = 0.0f;
    int psnr_count_class0 = 0;

    float psnr_sum_class1 = 0.0f;
    int psnr_count_class1 = 0;

    for(int classe = 0; classe < 2; ++classe) {
        for(const auto& entry : fs::directory_iterator(test_dirs[classe])) {
            std::string chemin_image = entry.path().string();

            OCTET* vecteur_caracteristiques = new OCTET[540];

            int classe_predite = traiter_image(chemin_image, W, vecteur_caracteristiques);

            // Mise à jour des compteurs pour le F1-score
            if(classe_predite == 1 && classe == 1) {
                TP++;
            } else if(classe_predite == 1 && classe == 0) {
                FP++;
            } else if(classe_predite == 0 && classe == 0) {
                TN++;
            } else if(classe_predite == 0 && classe == 1) {
                FN++;
            }

            // Calcul du PSNR
            float psnr = 0.0f;
            if(classe == 0) {
                psnr = calculer_PSNR(vecteur_caracteristiques, imagette_moyenne_class0, 540);
                psnr_sum_class0 += psnr;
                psnr_count_class0++;
            } else {
                psnr = calculer_PSNR(vecteur_caracteristiques, imagette_moyenne_class1, 540);
                psnr_sum_class1 += psnr;
                psnr_count_class1++;
            }

            delete[] vecteur_caracteristiques; // Libération de la mémoire

            // Affichage du résultat
            std::cout << "Image : " << chemin_image << " | Classe réelle : " << classe
                      << " | Classe prédite : " << classe_predite << " | PSNR : " << psnr << " dB" << std::endl;
        }
    }

    // Calcul du F1-score
    float precision = (TP + FP) > 0 ? (float)TP / (TP + FP) : 0;
    float recall = (TP + FN) > 0 ? (float)TP / (TP + FN) : 0;
    float f1_score = (precision + recall) > 0 ? 2 * (precision * recall) / (precision + recall) : 0;

    std::cout << "Précision : " << precision << std::endl;
    std::cout << "Rappel : " << recall << std::endl;
    std::cout << "F1-score : " << f1_score << std::endl;

    // Calcul des PSNR moyens
    float mean_psnr_class0 = psnr_count_class0 > 0 ? psnr_sum_class0 / psnr_count_class0 : 0.0f;
    float mean_psnr_class1 = psnr_count_class1 > 0 ? psnr_sum_class1 / psnr_count_class1 : 0.0f;

    std::cout << "PSNR moyen pour la classe 0 : " << mean_psnr_class0 << " dB" << std::endl;
    std::cout << "PSNR moyen pour la classe 1 : " << mean_psnr_class1 << " dB" << std::endl;

    return 0;
}


int traiter_image(const std::string& chemin_image, float W[2][540], OCTET* vecteur_caracteristiques) {
    OCTET* image;
    int nH, nW;
    lire_nb_lignes_colonnes_image_pgm(chemin_image.c_str(), &nH, &nW);
    allocation_tableau(image, OCTET, nH * nW);
    lire_image_pgm(chemin_image.c_str(), image, nH * nW);

    OCTET* images_filtrees[5];
    int filtree_nH = nH - 2;
    int filtree_nW = nW - 2;
    int taille_image_filtree = filtree_nH * filtree_nW;
    for(int i = 0; i < 5; ++i) {
        allocation_tableau(images_filtrees[i], OCTET, taille_image_filtree);
        memset(images_filtrees[i], 0, taille_image_filtree * sizeof(OCTET));
    }
    appliquer_filtre_3x3(image, images_filtrees[0], fpb_1, nH, nW);
    appliquer_filtre_3x3(image, images_filtrees[1], fpb_2, nH, nW);
    appliquer_filtre_3x3(image, images_filtrees[2], fph_1, nH, nW);
    appliquer_filtre_3x3(image, images_filtrees[3], fph_2, nH, nW);
    appliquer_filtre_3x3(image, images_filtrees[4], f_random, nH, nW);

    for(int i = 0; i < 5; ++i) {
        reLU(images_filtrees[i], taille_image_filtree);
    }

    OCTET* images_pooled[5];
    int pooled_nH = (nH - 2) / 2;
    int pooled_nW = (nW - 2) / 2;
    int taille_image_pooled = pooled_nH * pooled_nW;
    for(int i = 0; i < 5; ++i) {
        allocation_tableau(images_pooled[i], OCTET, taille_image_pooled);
        memset(images_pooled[i], 0, taille_image_pooled * sizeof(OCTET));
        max_pooling_2x2(images_filtrees[i], images_pooled[i], filtree_nH, filtree_nW);
    }

    OCTET* images_filtrees_couche2[15];
    int filtree_nH_couche2 = pooled_nH - 2;
    int filtree_nW_couche2 = pooled_nW - 2;
    int taille_image_filtree_couche2 = filtree_nH_couche2 * filtree_nW_couche2;

    for(int i = 0; i < 15; ++i) {
        allocation_tableau(images_filtrees_couche2[i], OCTET, taille_image_filtree_couche2);
        memset(images_filtrees_couche2[i], 0, taille_image_filtree_couche2 * sizeof(OCTET));
    }

    int index = 0;
    short* filtres_couche2[3] = {filtre2_fpb, filtre2_fph, filtre2_random};
    for(int f = 0; f < 3; ++f) {
        for(int c = 0; c < 5; ++c) {
            appliquer_filtre_3x3(images_pooled[c], images_filtrees_couche2[index++], filtres_couche2[f], pooled_nH, pooled_nW);
        }
    }

    for(int i = 0; i < 15; ++i) {
        reLU(images_filtrees_couche2[i], taille_image_filtree_couche2);
    }

    OCTET* images_pooled_couche2[15];
    int pooled_nH_couche2 = filtree_nH_couche2 / 2;
    int pooled_nW_couche2 = filtree_nW_couche2 / 2;
    int taille_image_pooled_couche2 = pooled_nH_couche2 * pooled_nW_couche2;

    for(int i = 0; i < 15; ++i) {
        allocation_tableau(images_pooled_couche2[i], OCTET, taille_image_pooled_couche2);
        memset(images_pooled_couche2[i], 0, taille_image_pooled_couche2 * sizeof(OCTET));
        max_pooling_2x2(images_filtrees_couche2[i], images_pooled_couche2[i], filtree_nH_couche2, filtree_nW_couche2);
    }

    int idx = 0;
    for(int i = 0; i < 15; ++i) {
        for(int y = 0; y < pooled_nH_couche2; ++y) {
            for(int x = 0; x < pooled_nW_couche2; ++x) {
                vecteur_caracteristiques[idx++] = images_pooled_couche2[i][indiceImage(y, x, pooled_nW_couche2)];
            }
        }
    }
    
    float scores[2];
    for(int i = 0; i < 2; ++i) {
        scores[i] = 0.0f;
        for(int j = 0; j < 540; ++j) {
            scores[i] += W[i][j] * vecteur_caracteristiques[j];
        }
    }

    // Softmax
    float exp_scores[2];
    float sum_exp = 0.0f;
    for(int i = 0; i < 2; ++i) {
        exp_scores[i] = exp(scores[i]);
        sum_exp += exp_scores[i];
    }

    float probas[2];
    for(int i = 0; i < 2; ++i) {
        probas[i] = exp_scores[i] / sum_exp;
    }

    int classe_predite = (probas[0] > probas[1]) ? 0 : 1;

    free(image);
    for(int i = 0; i < 5; ++i) {
        free(images_filtrees[i]);
        free(images_pooled[i]);
    }
    for(int i = 0; i < 15; ++i) {
        free(images_filtrees_couche2[i]);
        free(images_pooled_couche2[i]);
    }

    return classe_predite;
}

float calculer_PSNR(OCTET* imagette_test, float* imagette_moyenne, int taille) {
    float mse = 0.0f;
    for(int i = 0; i < taille; ++i) {
        float diff = imagette_test[i] - imagette_moyenne[i];
        mse += diff * diff;
    }
    mse /= taille;

    if(mse == 0) {
        return INFINITY;
    } else {
        return 10.0f * log10((255.0f * 255.0f) / mse);
    }
}

void convertir_imagette_moyenne_en_OCTET(float* imagette_moyenne, OCTET* imagette_OCTET, int taille) {
    float min_val = imagette_moyenne[0];
    float max_val = imagette_moyenne[0];
    for(int i = 1; i < taille; ++i) {
        if(imagette_moyenne[i] < min_val) min_val = imagette_moyenne[i];
        if(imagette_moyenne[i] > max_val) max_val = imagette_moyenne[i];
    }

    float range = max_val - min_val;
    if(range == 0) range = 1;

    for(int i = 0; i < taille; ++i) {
        float normalized = (imagette_moyenne[i] - min_val) / range; // Valeur entre 0 et 1
        imagette_OCTET[i] = static_cast<OCTET>(normalized * 255);
    }
}
