// ----------------------------------------------------------------------------
// Filename        : image_ppm.c
// Description     :
// Created On      : Tue Mar 31 13:26:36 2005
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define allocation_tableau(nom, type, nombre) \
if( (nom = (type*) calloc (nombre, sizeof(type) ) ) == NULL ) \
{\
 printf("\n Allocation dynamique impossible pour un pointeur-tableau \n");\
 exit(1);\
}

typedef unsigned char OCTET;

/*===========================================================================*/
void ignorer_commentaires(FILE * f)
{
  unsigned char c;
  while((c=fgetc(f)) == '#')
    while((c=fgetc(f)) != '\n');
  fseek(f, -sizeof(unsigned char), SEEK_CUR);
}
/*===========================================================================*/


/*===========================================================================*/
void ecrire_image_ppm(char  nom_image[], OCTET *pt_image, int nb_lignes, int nb_colonnes)
{
   FILE *f_image;
   int taille_image = 3*nb_colonnes * nb_lignes;

   if( (f_image = fopen(nom_image, "wb")) == NULL)
      {
	 printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	 fprintf(f_image,"P6\r") ;                               /*ecriture entete*/
	 fprintf(f_image,"%d %d\r255\r", nb_colonnes, nb_lignes) ;

	 if( (fwrite((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
	     != (size_t)(taille_image))
	    {
	       printf("\nErreur d'ecriture de l'image %s \n", nom_image);
	       exit(EXIT_FAILURE);
	    }
	 fclose(f_image);
      }
}
/*===========================================================================*/

/*===========================================================================*/		
void lire_nb_lignes_colonnes_image_ppm(char nom_image[], int *nb_lignes, int *nb_colonnes)
{
   FILE *f_image;
   int max_grey_val;

   /* cf : l'entete d'une image .ppm : P6                   */
   /*				       nb_colonnes nb_lignes */
   /*    			       max_grey_val          */


   if( (f_image = fopen(nom_image, "rb")) == NULL)
      {
	 printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	 fscanf(f_image, "P6 ");
	 ignorer_commentaires(f_image);
	 fscanf(f_image, "%d %d %d%*c", nb_colonnes, nb_lignes, &max_grey_val);
	 fclose(f_image);
      }
}
/*===========================================================================*/
/*===========================================================================*/
void lire_image_ppm(char  nom_image[], OCTET *pt_image, int taille_image)
{
   FILE *f_image;
   int  nb_colonnes, nb_lignes, max_grey_val;
   taille_image=3*taille_image;

   if( (f_image = fopen(nom_image, "rb")) == NULL)
      {
	 printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	fscanf(f_image, "P6 ");
	ignorer_commentaires(f_image);
	fscanf(f_image, "%d %d %d%*c",
	       &nb_colonnes, &nb_lignes, &max_grey_val); /*lecture entete*/

	 if( (fread((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
	     !=  (size_t)(taille_image))
	    {
	       printf("\nErreur de lecture de l'image %s \n", nom_image);
	       exit(EXIT_FAILURE);
	    }
	 fclose(f_image);
      }
}

/*===========================================================================*/
/*===========================================================================*/

void planR(OCTET *pt_image, OCTET *src, int taille_image){
   int i;
   for (i=0; i<taille_image; i++){
      pt_image[i]=src[3*i];
      }
   }
   
/*===========================================================================*/
/*===========================================================================*/

void planV(OCTET *pt_image, OCTET *src, int taille_image){
   int i;
   for (i=0; i<taille_image; i++){
      pt_image[i]=src[3*i+1];
      }
   }   

/*===========================================================================*/
/*===========================================================================*/

void planB(OCTET *pt_image, OCTET *src, int taille_image){
   int i;
   for (i=0; i<taille_image; i++){
      pt_image[i]=src[3*i+2];
      }
   }
   
/*===========================================================================*/   
/*===========================================================================*/

void ecrire_image_pgm(char  nom_image[], OCTET *pt_image, int nb_lignes, int nb_colonnes)
{
   FILE *f_image;
   int taille_image = nb_colonnes * nb_lignes;

   if( (f_image = fopen(nom_image, "wb")) == NULL)
      {
	 printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	 fprintf(f_image,"P5\r") ;                               /*ecriture entete*/
	 fprintf(f_image,"%d %d\r255\r", nb_colonnes, nb_lignes) ;

	 if( (fwrite((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
	     != (size_t) taille_image)
	    {
	       printf("\nErreur de lecture de l'image %s \n", nom_image);
	       exit(EXIT_FAILURE);
	    }
	 fclose(f_image);
      }
}
/*===========================================================================*/

void lire_nb_lignes_colonnes_image_pgm(char nom_image[], int *nb_lignes, int *nb_colonnes)
{
   FILE *f_image;
   int max_grey_val;

   /* cf : l'entete d'une image .pgm : P5                    */
   /*				       nb_colonnes nb_lignes */
   /*    			       max_grey_val          */


   if( (f_image = fopen(nom_image, "rb")) == NULL)
      {
	 printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	 fscanf(f_image, "P5 ");
	 ignorer_commentaires(f_image);
	 fscanf(f_image, "%d %d %d%*c", nb_colonnes, nb_lignes, &max_grey_val);
	 fclose(f_image);
      }
}
/*===========================================================================*/
/*===========================================================================*/
void lire_image_pgm(char  nom_image[], OCTET *pt_image, int taille_image)
{
   FILE *f_image;
   int  nb_colonnes, nb_lignes, max_grey_val;

   if( (f_image = fopen(nom_image, "rb")) == NULL)
      {
	 printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	fscanf(f_image, "P5 ");
	ignorer_commentaires(f_image);
	fscanf(f_image, "%d %d %d%*c",
	       &nb_colonnes, &nb_lignes, &max_grey_val); /*lecture entete*/

	 if( (fread((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
	     !=  (size_t) taille_image)
	    {
	       printf("\nErreur de lecture de l'image %s \n", nom_image);
	       exit(EXIT_FAILURE);
	    }
	 fclose(f_image);
      }
}
/*===========================================================================*/
int indiceImage(int x, int y, int width) {
   if (x >= width) {
      printf("x trop grand/n");
   }
   return y * width + x;
}
/*===========================================================================*/

/*===========================================================================*/
int indiceImageCouleur(char C, int x, int y, int width) {

   int indice;

   switch (C)
   {
      case 'R':
         indice = 0;
      break;

      case 'G':
         indice = 1;
      break;

      case 'B':
         indice = 2;
      break;

      default:
         perror("Canal non précisé");

   }

   return (3 * ((y * width) + x)) + indice;

}
/*===========================================================================*/

/*===========================================================================*/
void erosion_binaire(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW) {

   for(int x = 1; x < nW - 1; x++) { // Bons indices pour respecter les problèmes sur les bords
        for(int y = 1; y < nH - 1; y++) {
            int vDroite = ImgIn[indiceImage(x + 1, y, nW)];
            int vGauche = ImgIn[indiceImage(x - 1, y, nW)];
            int vBas = ImgIn[indiceImage(x, y + 1, nW)];
            int vHaut = ImgIn[indiceImage(x, y - 1, nW)];

            if(vDroite || vGauche || vHaut || vBas) {
               ImgOut[indiceImage(x, y, nW)] = 255;
            }

            else {
               ImgOut[indiceImage(x, y, nW)] = 0;
            }

        }
    }

}
/*===========================================================================*/

/*===========================================================================*/
void dilatation_binaire(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW) {

   for(int x = 1; x < nW - 1; x++) { // Bons indices pour respecter les problèmes sur les bords
        for(int y = 1; y < nH - 1; y++) {

            int vDroite = ImgIn[indiceImage(x + 1, y, nW)];
            int vGauche = ImgIn[indiceImage(x - 1, y, nW)];
            int vBas = ImgIn[indiceImage(x, y + 1, nW)];
            int vHaut = ImgIn[indiceImage(x, y - 1, nW)];

            if((vDroite == 0 || vGauche == 0 || vHaut == 0 || vBas == 0)) {
                ImgOut[indiceImage(x, y, nW)] = 0;
            }

            else {
                ImgOut[indiceImage(x, y, nW)]  = 255;
            }

        }

    }
}
/*===========================================================================*/

/*===========================================================================*/
double moyenne_pgm(OCTET *ImgIn, int nH, int nW) {
    int nTaille = nH * nW;
    double sum = 0;
    for(int i = 0; i < nH; i++) {
        for(int j = 0; j < nW; j++) {
            sum += ImgIn[indiceImage(i,j,nW)];
        }
    }
    return sum / nTaille;
}
/*===========================================================================*/

/*===========================================================================*/
double variance_pgm(OCTET *ImgIn, int nH, int nW) {
    int nTaille = nH * nW;
    double moyenne = moyenne_pgm(ImgIn, nH, nW); 
    double somme_carres_ecart = 0;
    for(int i = 0; i < nH; i++) {
        for(int j = 0; j < nW; j++) {
            double ecart = ImgIn[indiceImage(i,j,nW)] - moyenne;
            somme_carres_ecart += ecart * ecart;
        }
    }
    return somme_carres_ecart / nTaille;
}
/*===========================================================================*/

/*===========================================================================*/
double moyenne_bloc(OCTET *ImgIn, int x_debut_bloc, int x_fin_bloc, int y_debut_bloc, int y_fin_bloc, int nW) {
   int somme = 0;
   int taille_bloc = (x_fin_bloc - x_debut_bloc) * (y_fin_bloc - y_debut_bloc);
   for(int i = y_debut_bloc; i < y_fin_bloc; i++) {
      for(int j = x_debut_bloc; j < x_fin_bloc; j++) {
         somme += ImgIn[indiceImage(i,j,nW)];
      }
   }
   return (double)somme / taille_bloc;
}
/*===========================================================================*/

/*===========================================================================*/
double variance_bloc(OCTET *ImgIn, double moyenne, int x_debut_bloc, int x_fin_bloc, int y_debut_bloc, int y_fin_bloc, int nW) {
    double somme_carres_ecart = 0;
    int largeur_bloc = x_fin_bloc - x_debut_bloc;
    int hauteur_bloc = y_fin_bloc - y_debut_bloc;
    int taille_bloc = largeur_bloc * hauteur_bloc;
    for (int y = y_debut_bloc; y < y_fin_bloc; y++) {
        for (int x = x_debut_bloc; x < x_fin_bloc; x++) {
            double ecart = ImgIn[indiceImage(x,y,nW)] - moyenne;
            somme_carres_ecart += ecart * ecart;
        }
    }
    return somme_carres_ecart / taille_bloc;
}
/*===========================================================================*/

/*===========================================================================*/
void moyenne_bloc_couleur(OCTET *ImgIn, int x_debut, int x_fin, int y_debut, int y_fin, int nW, double &moyenneR, double &moyenneG, double &moyenneB) {
    long sommeR = 0, sommeG = 0, sommeB = 0;
    int taille_bloc = (x_fin - x_debut) * (y_fin - y_debut);
    
    for(int y = y_debut; y < y_fin; y++) {
        for(int x = x_debut; x < x_fin; x++) {
            sommeR += ImgIn[indiceImageCouleur('R', x, y, nW)];
            sommeG += ImgIn[indiceImageCouleur('G', x, y, nW)];
            sommeB += ImgIn[indiceImageCouleur('B', x, y, nW)];
        }
    }
    moyenneR = (double)(sommeR) / taille_bloc;
    moyenneG = (double)(sommeG) / taille_bloc;
    moyenneB = (double)(sommeB) / taille_bloc;
}
/*===========================================================================*/

/*===========================================================================*/
void variance_bloc_couleur(OCTET* ImgIn, int x_debut, int x_fin, int y_debut, int y_fin, int width, double moyenneR, double moyenneG, double moyenneB, double &varianceR, double &varianceG, double &varianceB) {
    long sommeR = 0, sommeG = 0, sommeB = 0;
    int taille_bloc = (x_fin - x_debut) * (y_fin - y_debut);
    
    for(int y = y_debut; y < y_fin; y++) {
        for(int x = x_debut; x < x_fin; x++) {
            sommeR += pow(ImgIn[indiceImageCouleur('R', x, y, width)] - moyenneR, 2);
            sommeG += pow(ImgIn[indiceImageCouleur('G', x, y, width)] - moyenneG, 2);
            sommeB += pow(ImgIn[indiceImageCouleur('B', x, y, width)] - moyenneB, 2);
        }
    }
    
    varianceR = (double)(sommeR) / taille_bloc;
    varianceG = (double)(sommeG) / taille_bloc;
    varianceB = (double)(sommeB) / taille_bloc;
}
/*===========================================================================*/

/*===========================================================================*/
double calculer_entropie(OCTET* ImgIn, int nH, int nW, int nTaille) {
    int T[256] = {0};

    for(int i = 0; i < nTaille; i++) {
        T[ImgIn[i]]++;
    }

    double entropie = 0.0;
    for(int i = 0; i < 256; i++) {
        if(T[i] > 0) {
            double alpha_i = (double)T[i] / nTaille; 
            entropie += alpha_i * log2(alpha_i);
        }
    }
    entropie = -entropie;
    return entropie;
}
/*===========================================================================*/

/*===========================================================================*/
double PSNR(OCTET* ImgOriginale, OCTET* ImgCompressee, int nTaille) {
   double MSE = 0.0;
   for(int i = 0; i < nTaille; i++) {
      MSE += pow(ImgOriginale[i] - ImgCompressee[i],2);
   }
   MSE /= nTaille;

   double PSNR = 10.0 * log10((255 * 255) / MSE);
   return PSNR;
}
/*===========================================================================*/

/*===========================================================================*/
void histo(OCTET* ImgIn, char nom[], int nH, int nW, int nTaille){
    int occurence[256];
    for(int i=0;i<256;i++){
        occurence[i]=0;
    }
    int indice;

    for (int i=0; i < nH; i++){
       for (int j=0; j < nW; j++){
          indice = ImgIn[i*nW+j];
          occurence[indice]= occurence[indice] + 1;
        }
    }

    ofstream histo;
    string nomFichier = nom; // Convertir le char[] en string
    nomFichier = nomFichier.substr(0, nomFichier.size() - 4); // Pour supprimer les extensions comme .pgm et .ppm du nom du fichier
    nomFichier += "_histo.dat"; // Concaténer avec la chaîne "histo.dat"
    histo.open(nomFichier);

        for (int i=0; i < 256 ; i++)
        {
            histo<<i<<" "<<occurence[i]<<endl;
        }


    histo.close();
}
/*===========================================================================*/