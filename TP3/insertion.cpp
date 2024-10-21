#include "image_ppm.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

void generer_sequence_bits_aleatoires(OCTET* message, int taille) {
    srand(time(0));
    for (int i = 0; i < taille; ++i) {
        message[i] = rand() % 2;
    }
}

void inserer_message_binaire(OCTET* image, OCTET* message, int taille, int k) {
    for (int i = 0; i < taille; ++i) {
        image[i] = (image[i] & ~(1 << k)) | (message[i] << k);
    }
}

double calculer_psnr(OCTET* img1, OCTET* img2, int taille) {
    double mse = 0.0;
    for (int i = 0; i < taille; ++i) {
        mse += pow(img1[i] - img2[i], 2);
    }
    mse /= taille;
    if (mse == 0) return 100; 
    return 10 * log10((255 * 255) / mse);
}

int main(int argc, char* argv[]) {
    char nomImgLue[250], nomImgEcriteMSB[250], nomImgEcriteLSB[250];
    int nH, nW, nTaille;

    if (argc != 4) {
        printf("Utilisation : %s image_in.pgm image_out_msb.pgm image_out_lsb.pgm\n", argv[0]);
        exit(1);
    }

    sscanf(argv[1], "%s", nomImgLue);
    sscanf(argv[2], "%s", nomImgEcriteMSB);
    sscanf(argv[3], "%s", nomImgEcriteLSB);

    OCTET *ImgIn, *ImgMSB, *ImgLSB, *Message;

    lire_nb_lignes_colonnes_image_pgm(nomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(nomImgLue, ImgIn, nTaille);
    allocation_tableau(ImgMSB, OCTET, nTaille);
    allocation_tableau(ImgLSB, OCTET, nTaille);
    allocation_tableau(Message, OCTET, nTaille);

    // Générer la séquence aléatoire de bits
    generer_sequence_bits_aleatoires(Message, nTaille);

    // Copier l'image originale dans ImgMSB et ImgLSB pour insertion du message
    memcpy(ImgMSB, ImgIn, nTaille * sizeof(OCTET));
    memcpy(ImgLSB, ImgIn, nTaille * sizeof(OCTET));

    // Insérer le message dans les MSB (bit 7)
    inserer_message_binaire(ImgMSB, Message, nTaille, 7);
    ecrire_image_pgm(nomImgEcriteMSB, ImgMSB, nH, nW);

    // Insérer le message dans les LSB (bit 0)
    inserer_message_binaire(ImgLSB, Message, nTaille, 0);
    ecrire_image_pgm(nomImgEcriteLSB, ImgLSB, nH, nW);

    // Calcul du PSNR
    double psnrMSB = calculer_psnr(ImgIn, ImgMSB, nTaille);
    double psnrLSB = calculer_psnr(ImgIn, ImgLSB, nTaille);

    histo(ImgLSB, nomImgEcriteLSB, nH, nW, nTaille);

    std::cout << "PSNR (MSB substitution) : " << psnrMSB << std::endl;
    std::cout << "PSNR (LSB substitution) : " << psnrLSB << std::endl;

    free(ImgIn); 
    free(ImgMSB); 
    free(ImgLSB); 
    free(Message);

    return 0;
}