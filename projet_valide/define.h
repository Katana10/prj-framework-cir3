#ifndef DEFINE_H
#define DEFINE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



/*! ------------------------------------------------------------

  \struct absorp

  \brief measures

  ------------------------------------------------------------*/

typedef struct{
		float acr;  /*!< AC R */
		float dcr;  /*!< DC R */
		float acir;  /*!< AC IR */
		float dcir;  /*!< DC IR */
	} absorp;

/*! ------------------------------------------------------------

  \struct oxy

  \brief

  ------------------------------------------------------------*/

typedef struct{
		int spo2;  /*!< SPO2 */
		int pouls;  /*!< Pouls */
	} oxy;

/*! ------------------------------------------------------------

  \struct point

  \brief

  ------------------------------------------------------------*/

typedef struct{
	int acMin;	/*!< Valeurs de AC */
	int acMax;
	int acirMin;	/*!< Valeurs de DC */
	int acirMax;

	int nb_points;
	int nb_points_par_periode;

} point;



#endif // DEFINE_H
