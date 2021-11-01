#include "lecture.h"

/*******************************************************
Nom fonction  : lecture
Role ........ : Permet lire les données d'un fichier
								sous format texte
Auteur ...... : Killian Talibart, Lucas Vaillant
********************************************************/
absorp lecture(FILE* file_pf, int* file_state){
	int i = 0;
	int j = 0;
	absorp myAbsorp;
	char champ[21] = {0};
	//On récupère toute la ligne
	for(i=0 ; i<21 ; i++){
		temp[i] = fgetc(file_pf);
		printf("%c\n",temp[i]);
		//On verifie que la trame n'est pas trop courte
		if(temp[i] == (char)10 && i <19){
			fgetc(file_pf);
			i = 0;
		}
		//Si on est à la fin du fichier on affecte EOF à file_state
		if(temp[i] == EOF){
			*file_state = EOF;
		}
	}

	if(*file_state != EOF){
		//On prend le caractère de champ[X] on lui retranche 48 pour avoir la valeur décimale
		//puis on le multiplie selon sa position et on centre à 0 en faisant -2047
		//(par ex: champ[0] correspond au chiffre des milliers donc on fait *1000)
		myAbsorp.acr = ((champ[0]-48)*1000 + (champ[1]-48)*100 + (champ[2]-48)*10 + champ[3]-48)-2048;
		myAbsorp.acir = ((champ[10]-48)*1000 + (champ[11]-48)*100 + (champ[12]-48)*10 + champ[13]-48)-2048;
		myAbsorp.dcr = (champ[5]-48)*1000 + (champ[6]-48)*100 + (champ[7]-48)*10 + champ[8]-48;
		myAbsorp.dcir = (champ[15]-48)*1000 + (champ[16]-48)*100 + (champ[17]-48)*10 + champ[18]-48;
	}
	return myAbsorp;
}

//On verifie que la trame sois bonne
/*
int verifTrame(char trame){
	int modele[21] = {48,48,48,48,44,48,48,48,48,44,48,48,48,48,44,48,48,48,48,10,13};
	int valide = 0;
	for(i=0; i<21 ; i++){
		if(!((i+1)%5) && champ[i] != modele[i] || i == 20){
			valide = -1;
		}else if(champ[i] < modele[i] && champ[i] > modele[i]+9){
			valide = -1;
		}else{
			valide +=  0;
		}
	}
	return valide;
}
*/
