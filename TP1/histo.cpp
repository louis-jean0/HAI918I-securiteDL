#include <stdio.h>
#include "image_ppm.h"  

int main(int argc, char* argv[]) {

    if(argc != 3) {
        printf("Utilisation : %s image.pgm fichier_sortie.dat\n",argv[0]);
        exit(1);
    }

    int T[256]; // Tableau d'occurences pour les 256 niveaux de gris
    memset(T,0,sizeof(T));

    int nH,nW,nTaille; //nH taille d'une colonne (nb de pixels sur une colonne), nW taille d'une ligne (nb de pixels sur une ligne), nTaille taille totale
    
    char nomImgLue[256];
    char nomFichierSortie[256];

    OCTET *ImgIn;

    sscanf(argv[1],"%s",nomImgLue);
    sscanf(argv[2],"%s",nomFichierSortie);

    lire_nb_lignes_colonnes_image_pgm(nomImgLue,&nH,&nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn,OCTET,nTaille);
    lire_image_pgm(nomImgLue,ImgIn,nTaille);

    for(int k = 0; k < 256; k++) {
        for(int i = 0; i < nH; i++) {
            for(int j = 0; j < nW; j++) {
                if(ImgIn[i*nW+j] == k) {
                    T[k]++;
                }
            }
        }
    }

    FILE *out = fopen(nomFichierSortie,"wb");

    if(out == NULL) {
        perror("Erreur lors de la création du fichier");
        exit(1);
    }

    fprintf(out,"# NDG Occurrences\n");

    for(int i = 0; i < 256; i++) {
        fprintf(out,"%d %d\n",i,T[i]);
    }

    fclose(out);

    return 0;

}