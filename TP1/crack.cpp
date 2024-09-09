#include "image_ppm.h"
#include <cstdlib>

int main(int argc, char* argv[]) {

    char nomImgLue[250],nomImgEcrite[250];
    int nH,nW,nTaille;

    if(argc != 3) {
        printf("Utilisation : %s image_in.pgm image_out.pgm",argv[0]);
        exit(1);
    }

    sscanf(argv[1],"%s",nomImgLue);
    sscanf(argv[2],"%s",nomImgEcrite);

    OCTET *ImgIn, *ImgOut, *ImgCandidate;

    lire_nb_lignes_colonnes_image_pgm(nomImgLue,&nH,&nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn,OCTET,nTaille);
    lire_image_pgm(nomImgLue,ImgIn,nTaille);
    allocation_tableau(ImgOut,OCTET,nTaille);
    allocation_tableau(ImgCandidate,OCTET,nTaille);

    double entropie_minimale = 10; // > 8 donc ok
    double entropie;
    int cle;
    for(int i = 0; i < 256; ++i) {
        srand(i);
        OCTET k = rand() % 256;
        ImgCandidate[0] = (ImgIn[0] - k) % 256;
        for(int i = 1; i < nTaille; ++i) {
            k = rand() % 256;
            ImgCandidate[i] = (ImgIn[i] - ImgIn[i-1] - k) % 256;
        }
        entropie = calculer_entropie(ImgCandidate, nH, nW, nTaille);
        if(entropie < entropie_minimale) {
            for(int i = 0; i < nTaille; ++i) {
                ImgOut[i] = ImgCandidate[i];
            }
            cle = i;
            entropie_minimale = entropie;
        }
    }

    ecrire_image_pgm(nomImgEcrite,ImgOut,nH,nW);
    std::cout<<"Entropie candidate : "<<entropie_minimale<<std::endl;
    std::cout<<"Clé candidate : "<<cle<<std::endl;

    free(ImgIn);free(ImgOut);

    return 0;

}