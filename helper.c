//Postelnicu Cristina-Marina 313CA 2023-2024
#include "helper.h"

/*functia int CLAMP are rolul de a se asigura ca elementele din matrice
sunt intre 0 si 255*/
int CLAMP(int x)
{
	if (x < 0)
		return 0;
	if (x > 255)
		return 255;
	return x;
}

/*functia ATOI_COORDINATES are rolul de a transforma coordonatele aflate intial
in format char, in date de tip int*/
int ATOI_COORDINATES(char x[])
{
	int X;
	if (atoi(x)) {
		X = atoi(x);
		return X;
	}
	if (strcmp(x, "0") == 0)
		X = 0;
	else
		return -999;
	return 0;
}