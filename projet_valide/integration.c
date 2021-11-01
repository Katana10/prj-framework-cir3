#include "integration.h"
#include "iir.h"
#include "fir.h"
#include "mesure.h"
#include "fichiers.h"
#include "affichage.h"
#include "lecture.h"

/*******************************************************
Nom fonction  : integrationTest
Role ........ : Execute toutes les fonctions, on va donc
                lire, filtrer et traiter les données
Auteur ...... : Killian Talibart, Lucas Vaillant
********************************************************/

void integrationTest(char* filename)
{
  absorp myAbsorp;
  oxy myOxy;
  point* valeurs_min_max;
  point* variable_temporaire;
  absorp* val_acr_post;
  int etat = 0;
  absorp tabAbsorp[51] = {0}; //Tableau de absorp qui va servir de mémoire pour la fonction fir

  //On initialise à 0 ces 3 variables
  valeurs_min_max = init_point();
  variable_temporaire = init_point();
  val_acr_post = init_iir();
  absorp* param_init = init_iir(); //On initialise à 0 un pointeur de absorp pour la fonction iir
  absorp temp;
  FILE* myFile = initFichier(filename);
  myOxy.pouls=0;
  myOxy.spo2=0;
  temp = lireFichier(myFile, &etat); //On lit une ligne du fichier
  //Boucle qui applique les filtres sur les données rentrées, on sort dès qu'on arrive à la fin du fichier
  do{
    valeurs_min_max->nb_points++;
    if(etat != EOF){
      myAbsorp = fir(temp, &tabAbsorp[0]);  //On filtre les données avec le filtre fir
      myAbsorp = iir(myAbsorp, param_init); //On filtre les données en sortie de fir avec le filtre iir
      mesure(&myOxy ,myAbsorp, valeurs_min_max, variable_temporaire, val_acr_post); //On récupère les mesures de spo2 et de pouls
      //printf("spo2 %d, pouls %d\n", myOxy.spo2, myOxy.pouls);
      affichage(myOxy); //On écrit dans un fichier pour qu'ensuite l'afficheur java affiche les données
    }
    printf("%.f,%.f,%.f,%.f\nspo2 : %d pouls : %d\n",myAbsorp.acr,myAbsorp.dcr,myAbsorp.acir,myAbsorp.dcir, myOxy.spo2, myOxy.pouls);
    temp = lireFichier(myFile, &etat); //On lit une ligne du fichier

  }while(etat != EOF);

  finFichier(myFile);
}
