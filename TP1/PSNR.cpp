#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 3) {
        printf("Utilisation : %s image_originale.ppm image_compressee.ppm\n",argv[0]);
        exit(1);
    }

    int nH, nW, nTaille;
    char nomImageOriginale[256], nomImageCompressee[256];

    sscanf(argv[1],"%s",nomImageOriginale);
    sscanf(argv[2],"%s",nomImageCompressee);

    OCTET *ImgOriginale, *ImgCompressee;

    lire_nb_lignes_colonnes_image_ppm(nomImageOriginale, &nH, &nW); // Images de même taille donc pas de différence

    nTaille = nH * nW;
    int nTaille3 = 3 * nTaille;

    allocation_tableau(ImgOriginale,OCTET,nTaille3);
    allocation_tableau(ImgCompressee,OCTET,nTaille3);

    lire_image_ppm(nomImageOriginale, ImgOriginale, nTaille);
    lire_image_ppm(nomImageCompressee, ImgCompressee, nTaille);

    double MSE = 0.0;
    for(int i = 0; i < nTaille3; i++) {
        MSE += pow(ImgOriginale[i] - ImgCompressee[i],2);
    }
    MSE /= nTaille3;

    double PSNR = 10.0 * log10((255 * 255) / MSE);

    std::cout<<"PSNR : "<<PSNR<<"dB"<<std::endl;

    return 0;
    
}