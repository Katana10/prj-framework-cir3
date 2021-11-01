#include "iir.h"
#include <string.h>


/*******************************************************
Nom fonction  : iirTest
Role ........ : Permet de filtrer toutes les données
								d'un fichier
Auteur ...... : Killian Talibart, Lucas Vaillant
********************************************************/

absorp iirTest(char* filename){
	//variables
	absorp	myAbsorp;
	absorp 	filtrer; 	// la (future) nouvelle valeurs après filtrage
	absorp*	post; 		// variable qui sert a garder les valeurs du précédent
	int etat = 0; 		// variable qui nous sert à savoir quand on a finit de lire le fichier

	// appel de la fonction init_iir pour initialiser à 0 le pointeur;
	post = init_iir();

	// ouverture et lecture du fichier grace aux fonctions fournis
	FILE* file = initFichier(filename);
	myAbsorp = lireFichier(file, &etat);	//extraction des absorp grâce à lireFichier

	while(etat !=-1){
		myAbsorp = iir(myAbsorp,post);
		filtrer = myAbsorp;
		//printf("données: %.0f,%0.f,%0.f,%0.f\n",filtrer.acr,filtrer.dcr,filtrer.dcir,filtrer.acir);
		myAbsorp = lireFichier(file,&etat);
	}

	// on ferme le fichier
	finFichier(file);

	return filtrer;

}

absorp* init_iir(){
	//initialisation du pointeur
	absorp* myIir;
	myIir=malloc(sizeof(absorp));

	//initialisation à 0
	myIir->acr=0;
	myIir->dcr=0;
	myIir->acir=0;
	myIir->dcir=0;
	return myIir;

}

/*******************************************************
Nom fonction  : iir
Role ........ : Permet de filtrer une ligne de données
Auteur ...... : Killian Talibart, Lucas Vaillant
********************************************************/
absorp iir(absorp myAbsorp, absorp* myIir){
	//variables
	float alpha=0.992;
	int valeur_R = myAbsorp.acr;
	int valeur_IR = myAbsorp.acir;

	//filtrage AC lumière Rouge
	//myAbsorp correspond à la valeur du fir correspondant
	myAbsorp.acr = (int)myAbsorp.acr - myIir->acr + alpha * myIir->dcr;
	//myIir correspond à la valeur du fir
	myIir->acr=valeur_R;
	myIir->dcr=myAbsorp.acr;

	//filtarge AC lumière InfraRouge
	myAbsorp.acir = (int)myAbsorp.acir - myIir->acir + alpha * myIir->dcir;
	myIir->acir=valeur_IR;
	myIir->dcir=myAbsorp.acir;

	return myAbsorp;
}
