#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {
    FILE* batch;
    for(size_t k = 1; k < 6; ++k) {
        char nom_batch[100];
        snprintf(nom_batch, sizeof(nom_batch), "data/data_batch_%01zu.bin", k);
        std::cout<<nom_batch<<std::endl;
        if((batch = fopen(nom_batch, "r")) == NULL) {
            std::cout<<"Pas d'accès en lecture sur le batch"<<std::endl;
            exit(EXIT_FAILURE);
        }
        for(size_t i = 0; i < 10000; ++i) {
            char num_image;
            fread(&num_image, sizeof(char), 1, batch);

            OCTET* image;
            allocation_tableau(image, OCTET, 3072);

            OCTET *R, *G, *B;
            allocation_tableau(R, OCTET, 1024);
            allocation_tableau(G, OCTET, 1024);
            allocation_tableau(B, OCTET, 1024);
            fread(R, sizeof(OCTET), 1024, batch);
            fread(G, sizeof(OCTET), 1024, batch);
            fread(B, sizeof(OCTET), 1024, batch);

            for(int j = 0; j < 1024; ++j) {
                image[3*j] = R[j];
                image[3*j+1] = G[j];
                image[3*j+2] = B[j]; 
            }

            char nom_image[100];
            snprintf(nom_image, sizeof(nom_image), "data/images/batch_%01zu_%05zu_%01d.ppm", k, i, static_cast<int>(num_image));
            ecrire_image_ppm(nom_image, image, 32, 32);

            free(image);
            free(R);
            free(G);
            free(B);
        }
        fclose(batch);
    }
}