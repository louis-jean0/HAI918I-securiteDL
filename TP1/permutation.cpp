#include "image_ppm.h"
#include <cstdlib>

int main(int argc, char* argv[]) {

    char nomImgLue[250],nomImgEcrite[250];
    int nH,nW,nTaille;

    if(argc != 4) {
        printf("Utilisation : %s image_in.pgm image_out.pgm clé",argv[0]);
        exit(1);
    }

    sscanf(argv[1],"%s",nomImgLue);
    sscanf(argv[2],"%s",nomImgEcrite);
    OCTET cle = atoi(argv[3]);

    OCTET *ImgIn,*ImgOut;

    lire_nb_lignes_colonnes_image_pgm(nomImgLue,&nH,&nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn,OCTET,nTaille);
    lire_image_pgm(nomImgLue,ImgIn,nTaille);
    allocation_tableau(ImgOut,OCTET,nTaille);

    int positions[nTaille];

    for(int i = 0; i < nTaille; ++i) {
        positions[i] = -1; // Initialisation à -1
    }

    srand(cle);
    for(int i = 0; i < nTaille; ++i) {
        int pos = rand() % nTaille;
        while(positions[pos] != -1) {
            if(pos == nTaille - 1) {
                pos = 0;
            }
            else {
                pos++;
            }
        }
        ImgOut[pos] = ImgIn[i];
        positions[pos] = i;
    }

    ecrire_image_pgm(nomImgEcrite,ImgOut,nH,nW);
    double psnr = PSNR(ImgIn, ImgOut, nTaille);
    std::cout<<"PSNR = "<<psnr<<std::endl;

    double entropie_originale = calculer_entropie(ImgIn, nH, nW, nTaille);
    double entropie_chiffree = calculer_entropie(ImgOut, nH, nW, nTaille);

    std::cout<<"Entropie originale : "<<entropie_originale<<std::endl;
    std::cout<<"Entropie chiffrée : "<<entropie_chiffree<<std::endl;

    histo(ImgIn, nomImgLue, nH, nW, nTaille);
    histo(ImgOut, nomImgEcrite, nH, nW, nTaille);

    free(ImgIn);free(ImgOut);

    return 0;

}