#include "define.h"

oxy mesureTest(char* filename);
void mesure(oxy* myOxy, absorp myAbsorp, point* valeurs_min_max, point* valeurs_sauve, absorp* val_acr_post);
point* init_point();
void mesureMinMax(absorp myAbsorp, point* valeurs_temporaire, point* valeurs_min_max);
void echange(point* valeurs_temporaire, point* valeurs_principale);
int spo2(point* value, absorp myAbsorp);
int pouls(point* value);
