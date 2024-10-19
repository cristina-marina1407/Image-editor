//Postelnicu Cristina-Marina 313CA 2023-2024
#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//definim structura pentru matricea rgb
typedef struct RGB_MATRIX {
	int r;
	int g;
	int b;
} RGB_MATRIX;

//definim structura imaginii
typedef struct IMAGE {
	char magic_word[3];
	int n, m;
	int max;
	int x1, x2, y1, y2;
	int **matrix;
	RGB_MATRIX **rgb;
} IMAGE;

int CLAMP(int x);
int ATOI_COORDINATES(char x[]);

#endif // HELPER_H