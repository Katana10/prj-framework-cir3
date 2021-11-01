#include "affichage.h"


/*******************************************************
Nom fonction  : affichage
Role ........ : Ecrit dans un fichier Data.txt,
								un fichier .verrouData est créé pour
          			empêcher d'autre logiciel de faire des
								actions sur le fichier en même temps
Auteur ...... : Killian Talibart, Lucas Vaillant
********************************************************/


void affichage(oxy myOxy){
	if( access( ".verrouData", F_OK ) != -1 ){
		// Fichier existe
	}else{
		// Fichier n'existe pas
		FILE* fdata = NULL;
		FILE* fverrouData = NULL;
		fdata = fopen("Data.txt","w");
		//On créé le fichier .verrouData pour avoir la main sur le fichier Data.txt
		fverrouData = fopen(".verrouData", "w");

		//Si le fichier data existe alors on ecrit dedans
		if (fdata != NULL){
			fprintf(fdata, "%d\n%d",myOxy.spo2, myOxy.pouls );
			//on ferme le fichier Data.txt
			fclose(fdata);
		}
		//On ferme puis supprime le fichier .verrourData
		//Cela va permettre à l'interface java de lire le fichier Data.txt
		fclose(fverrouData);
		remove(".verrouData");
	}

}
