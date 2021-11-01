#include "fir.h"
#include "fichiers.h"

/*******************************************************
Nom fonction  : firTest
Role ........ : Permet de filtrer toutes les données
								d'un fichier
Auteur ...... : Killian Talibart, Lucas Vaillant
********************************************************/

absorp firTest(char* filename){
	int etat = 0;
	int i = 0;
	absorp	myAbsorp;
	absorp temp;
	FILE* record1 = initFichier(filename);
	absorp tabAbsorp[51];	//tableau de absorp, permet de garder en mémoire les données
	//On initialise tabAbsorp à 0
	for(i=0; i<51 ; i++){
		tabAbsorp[i].acr = 0;
		tabAbsorp[i].dcr = 0;
		tabAbsorp[i].acir = 0;
		tabAbsorp[i].dcir = 0;
	}
	temp = lireFichier(record1, &etat);	//on recupère les valeurs d'une ligne

	do{
		//Si on est à la fin du fichier, on return myAbsorp
			myAbsorp = fir(temp, &tabAbsorp[0]); //On récupère la valeur filtré dans myAbsorp
			printf("%f,%f,%f,%f\n",myAbsorp.acr,myAbsorp.dcr,myAbsorp.acir,myAbsorp.dcir);
			temp = lireFichier(record1, &etat);	//on recupère les valeurs d'une ligne
	}while(etat != EOF);

	finFichier(record1);
	printf("\n%f,%f,%f,%f\n",myAbsorp.acr,myAbsorp.dcr,myAbsorp.acir,myAbsorp.dcir);
	return myAbsorp;

}


/*******************************************************
Nom fonction. : fir
Role ........ : Permet de filtrer une ligne de données
Auteur ...... : Killian Talibart, Lucas Vaillant
********************************************************/

absorp fir(absorp myAbsorp, absorp* tabAbsorp){
	//on declare les coefficients du filtre
	float FIR_TAPS[51]={
		1.4774946e-019,1.6465231e-004,3.8503956e-004,7.0848037e-004,
    1.1840522e-003,1.8598621e-003,2.7802151e-003,3.9828263e-003,
    5.4962252e-003,7.3374938e-003,9.5104679e-003,1.2004510e-002,
    1.4793934e-002,1.7838135e-002,2.1082435e-002,2.4459630e-002,
    2.7892178e-002,3.1294938e-002,3.4578348e-002,3.7651889e-002,
    4.0427695e-002,4.2824111e-002,4.4769071e-002,4.6203098e-002,
    4.7081811e-002,4.7377805e-002,4.7081811e-002,4.6203098e-002,
    4.4769071e-002,4.2824111e-002,4.0427695e-002,3.7651889e-002,
    3.4578348e-002,3.1294938e-002,2.7892178e-002,2.4459630e-002,
    2.1082435e-002,1.7838135e-002,1.4793934e-002,1.2004510e-002,
    9.5104679e-003,7.3374938e-003,5.4962252e-003,3.9828263e-003,
    2.7802151e-003,1.8598621e-003,1.1840522e-003,7.0848037e-004,
    3.8503956e-004,1.6465231e-004,1.4774946e-019
	};
	int i = 0;
	//On décale tous les éléments du tableau d'une case à gauche
	for(i=0; i<50 ; i++){
		tabAbsorp[i] = tabAbsorp[i+1];
	}
	//On ajoute la nouvelle donnée à l'indice 0 du tableau
	tabAbsorp[50] = myAbsorp;

	myAbsorp.acr = 0;
	myAbsorp.acir = 0;
	//On fait les calculs pour filtrer les données
	for(i=0; i<51 ; i++){
		 myAbsorp.acr += FIR_TAPS[i]*tabAbsorp[50-i].acr;
		 myAbsorp.acir += FIR_TAPS[i]*tabAbsorp[50-i].acir;
	}

	return myAbsorp;
}
