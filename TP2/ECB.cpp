#include "image_ppm.h"
#include "AES.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {

    char nomImgLue[250], nomImgChiffree[250], nomImgDechiffree[250];
    int nH, nW, nTaille;

    if(argc != 4) {
        printf("Utilisation : %s image_in.pgm image_chiffree.pgm image_dechiffree.pgm\n", argv[0]);
        exit(1);
    }

    sscanf(argv[1], "%s", nomImgLue);
    sscanf(argv[2], "%s", nomImgChiffree);
    sscanf(argv[3], "%s", nomImgDechiffree);

    unsigned char *ImgIn, *ImgChiffree, *ImgDechiffree;

    lire_nb_lignes_colonnes_image_pgm(nomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, unsigned char, nTaille);
    lire_image_pgm(nomImgLue, ImgIn, nTaille);
    allocation_tableau(ImgChiffree, unsigned char, nTaille);
    allocation_tableau(ImgDechiffree, unsigned char, nTaille);

    unsigned char key[16] = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f
    };

    AES aes(AESKeyLength::AES_128);
    unsigned char roundKeys[176]; // Pour AES-128, la taille est 176 octets
    aes.KeyExpansion(key, roundKeys);

    for(int i = 0; i < nH; i += 4) {
        for(int j = 0; j < nW; j+=4) {
            unsigned char bloc[16];
            for(int di = 0; di < 4; ++di) {
                for(int dj = 0; dj < 4; ++dj) {
                    int index = (i + di) * nW + (j + dj);
                    bloc[di * 4 + dj] = ImgIn[index];
                }
            }
            // Chiffrage
            unsigned char bloc_chiffre[16];
            aes.EncryptBlock(bloc, bloc_chiffre, roundKeys);
            for(int di = 0; di < 4; ++di) {
                for(int dj = 0; dj < 4; ++dj) {
                    int index = (i + di) * nW + (j + dj);
                    ImgChiffree[index] = bloc_chiffre[di * 4 + dj];
                }
            }
            // Déchiffrage
            unsigned char bloc_dechiffre[16];
            aes.DecryptBlock(bloc_chiffre, bloc_dechiffre, roundKeys);
            for(int di = 0; di < 4; ++di) {
                for(int dj = 0; dj < 4; ++dj) {
                    int index = (i + di) * nW + (j + dj);
                    ImgDechiffree[index] = bloc_dechiffre[di * 4 + dj];
                }
            }
        }
    }

    ecrire_image_pgm(nomImgChiffree, ImgChiffree, nH, nW);
    ecrire_image_pgm(nomImgDechiffree, ImgDechiffree, nH, nW);

    double psnr = PSNR(ImgIn, ImgChiffree, nTaille);
    std::cout<<"PSNR entre l'image originale et l'image chiffrée = "<<psnr<<" dB"<<std::endl;

    double entropie_originale = calculer_entropie(ImgIn, nH, nW, nTaille);
    double entropie_chiffree = calculer_entropie(ImgChiffree, nH, nW, nTaille);

    std::cout<<"Entropie originale : "<<entropie_originale<<std::endl;
    std::cout<<"Entropie chiffrée : "<<entropie_chiffree<<std::endl;

    histo(ImgIn, nomImgLue, nH, nW, nTaille);
    histo(ImgChiffree, nomImgChiffree, nH, nW, nTaille);

    free(ImgIn);
    free(ImgChiffree);
    free(ImgDechiffree);

    return 0;
}
