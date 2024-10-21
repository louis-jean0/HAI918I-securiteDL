#include "image_ppm.h"
#include <cstdlib>

void extraire_plan_binaire(OCTET* ImgIn, OCTET* ImgOut, int nTaille, int k) {
    // Pour chaque pixel de l'image, extraire le k-ième bit
    for (int i = 0; i < nTaille; ++i) {
        // Décaler le pixel de k positions à droite et récupérer le dernier bit
        ImgOut[i] = (ImgIn[i] >> k) & 1;
        // Multiplier par 255 pour visualiser (0 ou 255) si l'on souhaite sauvegarder en image
        ImgOut[i] *= 255;
    }
}

int main(int argc, char* argv[]) {
    char nomImgLue[250], nomImgEcrite[250];
    int nH, nW, nTaille;

    if (argc != 4) {
        printf("Utilisation : %s image_in.pgm image_out.pgm plan_binaire", argv[0]);
        exit(1);
    }                       

    sscanf(argv[1], "%s", nomImgLue);
    sscanf(argv[2], "%s", nomImgEcrite);
    int k = atoi(argv[3]);  // Le plan binaire à extraire

    if (k < 0 || k > 7) {
        printf("Erreur : Le plan binaire doit être entre 0 et 7\n");
        exit(1);
    }

    OCTET* ImgIn, * ImgOut;

    lire_nb_lignes_colonnes_image_pgm(nomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(nomImgLue, ImgIn, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // Extraire le k-ième plan binaire
    extraire_plan_binaire(ImgIn, ImgOut, nTaille, k);

    // Sauvegarder l'image du plan binaire
    ecrire_image_pgm(nomImgEcrite, ImgOut, nH, nW);

    histo(ImgOut, nomImgEcrite, nH, nW, nTaille);

    free(ImgIn);
    free(ImgOut);

    return 0;
}
