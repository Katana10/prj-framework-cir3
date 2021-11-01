#include "mesure.h"
#include "iir.h"
#include <math.h>

oxy mesureTest(char* filename){
	//variables
	absorp myAbsorp;
	point* valeurs_min_max;
	point* variable_temporaire;
	absorp* val_acr_post;
	oxy myOxy;
	int etat=0;

	//on initialise les valeurs à 0
	valeurs_min_max = init_point();
	variable_temporaire = init_point();
	val_acr_post = init_iir();

	// ouverture et lecture du fichier grace aux fonctions fournis
	FILE* file = initFichier(filename);
	myAbsorp = lireFichier(file, &etat);	//extraction des absorp grâce à lireFichier

	valeurs_min_max->nb_points_par_periode = 1;

	//calcul du spo2 et du pouls
	while(etat !=-1){
		//on augmente le nombre de point a chaque rentrée dans la boucle
		valeurs_min_max->nb_points++;
		mesure(&myOxy, myAbsorp, valeurs_min_max, variable_temporaire, val_acr_post);
		//sert à remplacer la valeur de la structure absorp
		//et à modifier la variable etat quand on est à la fin du fichier
		myAbsorp = lireFichier(file,&etat);
	}

	// on ferme le fichier
	finFichier(file);

	return myOxy;

}

/**
La fonction init_point sert comme son nom l'indique à initialiser un pointeur sur une structure de type point
en mettant toutes ses composantes à 0
**/
point* init_point(){
	//allocation mémoire pour un pointeur de point
	point* myPoint=NULL;
	myPoint = malloc(sizeof(point));
	//on initialise toutes les composantes à 0
	myPoint->acMin = 0;
	myPoint->acMax = 0;
	myPoint->acirMin = 0;
	myPoint->acirMax = 0;

	myPoint->nb_points = 0;
	myPoint->nb_points_par_periode = 0;

	return myPoint;
}

/**
La fonction mesure est la fonction principale de ce fichier elle gère tous les appels de fonction importants
**/
void mesure(oxy* myOxy, absorp myAbsorp, point* valeurs_min_max, point* variable_temporaire, absorp* val_acr_post){
	//variables
	// oxy myOxy;

	// la fonctions disribut la plus grande valeur entre myAbsorp.acr (variable lu en direct) et valeurs_min_max->acMax
	mesureMinMax(myAbsorp,variable_temporaire, valeurs_min_max );
	//On appel la fonction échange pour remettre les valeurs dans la variable principale
	echange(variable_temporaire, valeurs_min_max);
	// printf(" MAX %d,  MIN %d\n", valeurs_min_max->acMax, valeurs_min_max->acMin);
	valeurs_min_max->nb_points_par_periode =valeurs_min_max->nb_points;
	// on met les valeurs grace au calcul fournis
	// myOxy.spo2 = 0;
	// myOxy.pouls = 0;


	/**
	Ici on vérifie le passage d'une période à une autre mais dans un sens, on a choisit le sens montant
	tel qu'on divise une période comme suis
	0 front montant positif, MAX, front descendant positif, 0 ,front descendant négatif, MIN, front montant négatif, 0.
	c'est ce dernier front que l'on recherche car il nous suffit d'avoir 2 valeurs en mémoires
	**/
	if ((myAbsorp.acr >0 && val_acr_post->acr <0) && (valeurs_min_max->nb_points_par_periode !=1)){

		// on met les valeurs grâce au calculs fournis dans l'énoncé
		myOxy->spo2 = spo2(valeurs_min_max, myAbsorp);
		//on enregistre le nombre de points qu'on a dans une période qui correspond au nombre d'enregistrement
		valeurs_min_max->nb_points_par_periode = valeurs_min_max->nb_points;
		// avec ces données on peut calculer le pouls
		myOxy->pouls = pouls(valeurs_min_max);
		// On réinitialise la données temporaire à 0 pour la prochaine périod (pour limiter le risque d'erreur)
		variable_temporaire = init_point();
		// printf("oui %d, %d, %d, %d\n", variable_temporaire->acMin ,variable_temporaire->acMax ,variable_temporaire->acirMin ,variable_temporaire->acirMax);

		//comme on vient de finir une période on remet le compteur à 0 pour en démarrer une nouvelle
		printf("periode nb point%d, pouls %d , spo2 %d, MAX %d, MIN %d\n", valeurs_min_max->nb_points_par_periode, myOxy->pouls,myOxy->spo2, valeurs_min_max->acMax, valeurs_min_max->acMin);
		valeurs_min_max->acMin = 0;
		valeurs_min_max->acMax = 0;
		valeurs_min_max->acirMin = 0;
		valeurs_min_max->acirMax = 0;

		valeurs_min_max->nb_points = 0;
		valeurs_min_max->nb_points_par_periode = 0;


	}
	// ici on inscrit la valeur de l'acr de la structure absorp qui passe actuellement
	//dans le but de la comparer à celle d'apprès
	val_acr_post->acr = myAbsorp.acr;
	// val_acr_post->acr = myAbsorp.acir;

	// return myOxy;

}

/**
La fonction mesureMinMax mesure le maximum et le minimun entre deux valeurs
et les inserent dans une structure de types point elle nous sert donc pour faire le
Peak To Peak sur chaque lumière
**/
void mesureMinMax(absorp myAbsorp, point* temporaire, point* valeurs_min_max){

	//la fonction fmax sert à récuéper la valeur maximale entre deux nombres
	//la fonction fmin sert à récuéper la valeur minimale entre deux nombres
	//Lumière Rouge
	temporaire->acMax = fmax(myAbsorp.acr, valeurs_min_max->acMax);
	temporaire->acMin = fmin(myAbsorp.acr, valeurs_min_max->acMin);
	//Lumière InfraRouge
	temporaire->acirMax = fmax(myAbsorp.acir, valeurs_min_max->acirMax);
	temporaire->acirMin = fmin(myAbsorp.acir, valeurs_min_max->acMin);


}

/**
La fonctione echange permet de mettre les valeurs d'une structure de type point dans une autre
**/
void echange(point* variable_temporaire, point* variable_principale){


	//on remplace les composante min et max de chaque lumière
	//Lumière Rouge
	variable_principale->acMin = variable_temporaire->acMin;
	variable_principale->acMax = variable_temporaire->acMax;
	//Lumière InfraRouge
	variable_principale->acirMin = variable_temporaire->acirMin;
	variable_principale->acirMax = variable_temporaire->acirMax;



}

/**
La fonction spo2 sert à calculer le taux de saturation d'oxygène dans le sang
**/
int spo2(point* value, absorp myAbsorp){

	//Variables
	float rsIr;
	int spo2;

	// On réalise le calcul de l'énoncé le PtP correspondant à une différence entre le max et le min de chaque lumière
	rsIr = ((value->acMax - value->acMin)/myAbsorp.dcr)/((value->acirMax - value->acirMin)/myAbsorp.dcir);

		// Condition pour séparer les deux différentes partie de la fonction
		if(rsIr<0.4){
			spo2=100;
		}else if((rsIr<1) && (rsIr>0.4)){
			// 110 correspond à la valeur trouver tel que 100 = x-(25*0.4) c'est une ordonné à l'origine en quelque sorte
			// -(25) est le coefficient directeur de la droite affine entre le point (0.4, 100) et (1, 85)
			spo2 = (int)110 - (25 * rsIr);

		}else if (rsIr>3.4){
			spo2 =0;
		}else{
			// (1445/12) correspond à la valeur absolue trouver au point 3.4 tel que 0 = x - ((425/12) * 3.4) c'est une ordonné à l'origine en quelque sorte
			// -(425/12) est le coefficient directeur de la droite affine entre le point (1, 85) et (3.4, 0)
			spo2 = (int) (1445/12) - ((425/12) * rsIr);

		}

	return spo2;
}

/**
La fonction pouls sert à calculer le pouls
**/
int pouls(point* value){
	//Variable
	int pouls;
	/**Calcul grace aux données:  on cherche un BPM  => fréquance 1/T
	On à un nombre de point par période
	Qu'on multiplie par leur fréquence d'apparition (2 millisecondes) =>T
	Et comme on veut des minute et pas de millisecondes on transforme et on multiplie
	**/
	pouls = (1/(value->nb_points_par_periode*0.002))*60;
	// printf("pouls %d\n", pouls);
	return pouls;
}
