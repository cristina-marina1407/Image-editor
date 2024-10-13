//Postelnicu Cristina-Marina 313CA 2023-2024
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
void LOAD_ASCII(IMAGE *im, char magic_aux[], FILE *in_file, int *photo);
void LOAD_BINARY(IMAGE *im, char magic_aux[], FILE *in_file, int *photo);
void LOAD(IMAGE *im, int *photo);
void SELECT(IMAGE *im, int photo);
void EQUALIZE(IMAGE *im, int photo);
void CROP_MATRIX(IMAGE *im);
void CROP_RGB(IMAGE *im);
void CROP(IMAGE *im, int photo);
void APPLY_EDGE(IMAGE *im);
void APPLY_SHARPEN(IMAGE *im);
void APPLY_BLUR(IMAGE *im);
void APPLY_GAUSSIAN_BLUR(IMAGE *im);
void APPLY(IMAGE *im, int photo);
void SAVE_ASCII(IMAGE *im, FILE *out_file);
void SAVE_BINARY(IMAGE *im, FILE *out_file);
void SAVE(IMAGE *im, int photo);
void EXIT(IMAGE *im, int photo);

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

//functia LOAD_ASCII incarca in fisier imaginile cu matrice ascii
void LOAD_ASCII(IMAGE *im, char *magic_aux, FILE *in_file, int *photo)
{
	int i, j;
	//copiem valoarea lui magic_aux in im->magic_word
	strcpy(im->magic_word, magic_aux);
	if (strcmp(magic_aux, "P1") == 0) {
		fscanf(in_file, "%d %d", &im->n, &im->m);
		//alocam dinamic matricea
		im->matrix = malloc(im->m * sizeof(int *));
		if (!im->matrix)
			return;
		for (i = 0; i < im->m; i++) {
			im->matrix[i] = malloc(im->n * sizeof(int));
			if (!im->matrix[i]) {
				for (j = 0; j < i; j++)
					free(im->matrix[j]);
				free(im->matrix);
				return;
			}
		}
		//citim matricea
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++)
				fscanf(in_file, "%d", &im->matrix[i][j]);
		//daca o imagine este incarcata in fisier crestem contorul
		*photo = 1;
	//repetam procesul pentru matricea de tip P2, adaugand valoarea maxima
	} else if (strcmp(magic_aux, "P2") == 0) {
		fscanf(in_file, "%d %d", &im->n, &im->m);
		fscanf(in_file, "%d", &im->max);
		im->matrix = malloc(im->m * sizeof(int *));
		if (!im->matrix)
			return;
		for (i = 0; i < im->m; i++) {
			im->matrix[i] = malloc(im->n * sizeof(int));
			if (!im->matrix[i]) {
				for (j = 0; j < i; j++)
					free(im->matrix[j]);
				free(im->matrix);
				return;
			}
		}
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++)
				fscanf(in_file, "%d", &im->matrix[i][j]);
		*photo = 1;
	//repetam procesul pentru matricea de tip P3
	} else if (strcmp(magic_aux, "P3") == 0) {
		fscanf(in_file, "%d %d", &im->n, &im->m);
		fscanf(in_file, "%d", &im->max);
		im->rgb = malloc(im->m * sizeof(RGB_MATRIX *));
		if (!im->rgb)
			return;
		for (i = 0; i < im->m; i++) {
			im->rgb[i] = malloc(im->n * sizeof(RGB_MATRIX));
			if (!im->rgb[i]) {
				for (j = 0; j < i; j++)
					free(im->rgb[j]);
				free(im->rgb);
				return;
			}
		}
		/*un element al matricei este format din trei valori pentru rosu,
		verde si albastru*/
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++) {
				fscanf(in_file, "%d", &im->rgb[i][j].r);
				fscanf(in_file, "%d", &im->rgb[i][j].g);
				fscanf(in_file, "%d", &im->rgb[i][j].b);
			}
		*photo = 1;
	}
	//inchidem fisierul
	fclose(in_file);
}

//functia LOAD_BINARY incarca in fisier imaginile cu matrice binara
void LOAD_BINARY(IMAGE *im, char *magic_aux, FILE *in_file, int *photo)
{
	int i, j;
	//copiem valoarea lui magic_aux in im->magic_word
	strcpy(im->magic_word, magic_aux);
	if (strcmp(magic_aux, "P4") == 0) {
		fscanf(in_file, "%d %d\n", &im->n, &im->m);
		//alocam dinamic matricea
		im->matrix = malloc(im->m * sizeof(int *));
		if (!im->matrix)
			return;
		for (i = 0; i < im->m; i++) {
			im->matrix[i] = malloc(im->n * sizeof(int));
			if (!im->matrix[i]) {
				for (j = 0; j < i; j++)
					free(im->matrix[j]);
				free(im->matrix);
				return;
			}
		}
		/*pentru matricele binare folosim functia fread si
		transformam elementele in unsigned char*/
		char elements;
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++) {
				fread(&elements, sizeof(char), 1, in_file);
				im->matrix[i][j] = (unsigned char)elements;
			}
		*photo = 1;
	//repetam procesul pentru matricea de tip P5, adaugand valoarea maxima
	} else if (strcmp(magic_aux, "P5") == 0) {
		fscanf(in_file, "%d %d", &im->n, &im->m);
		fscanf(in_file, "%d\n", &im->max);
		im->matrix = malloc(im->m * sizeof(int *));
		if (!im->matrix)
			return;
		for (i = 0; i < im->m; i++) {
			im->matrix[i] = malloc(im->n * sizeof(int));
			if (!im->matrix[i]) {
				for (j = 0; j < i; j++)
					free(im->matrix[j]);
				free(im->matrix);
				return;
			}
		}
		char elements;
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++) {
				fread(&elements, sizeof(char), 1, in_file);
				im->matrix[i][j] = (unsigned char)elements;
			}
		*photo = 1;
	} else if (strcmp(magic_aux, "P6") == 0) {
		fscanf(in_file, "%d %d", &im->n, &im->m);
		fscanf(in_file, "%d\n", &im->max);
		im->rgb = malloc(im->m * sizeof(RGB_MATRIX *));
		if (!im->rgb)
			return;
		for (i = 0; i < im->m; i++) {
			im->rgb[i] = malloc(im->n * sizeof(RGB_MATRIX));
			if (!im->rgb[i]) {
				for (j = 0; j < i; j++)
					free(im->rgb[j]);
				free(im->rgb);
				return;
			}
		}
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++) {
				unsigned char elements;
				fread(&elements, sizeof(char), 1, in_file);
				im->rgb[i][j].r = (int)elements;
				fread(&elements, sizeof(char), 1, in_file);
				im->rgb[i][j].g = (int)elements;
				fread(&elements, sizeof(char), 1, in_file);
				im->rgb[i][j].b = (int)elements;
			}
		*photo = 1;
	}
	fclose(in_file);
}

//functia LOAD elibereaza memoria si incarca in fisier
void LOAD(IMAGE *im, int *photo)
{
	int i;
	/*eliberam memoria pentru o imagine incarcata anterior,
	verificand valoarea lui photo si tipul de matrice*/
	if (*photo == 1) {
		if (im->matrix) {
			for (i = 0; i < im->m; i++)
				free(im->matrix[i]);
			free(im->matrix);
			im->matrix = NULL;
		}
		if (im->rgb) {
			for (i = 0; i < im->m; i++)
				free(im->rgb[i]);
			free(im->rgb);
			im->rgb = NULL;
		}
	}
	//citim numele fisierului ce trebuie incarcat si il deschidem
	char file_name[100];
	scanf("%s", file_name);
	FILE *in_file = fopen(file_name, "rb");
	if (!in_file) {
		printf("Failed to load %s\n", file_name);
		*photo = 0;
		return;
	}
	//incepem citirra din fisier cu magic_aux, tipul imaginii
	char magic_aux[3];
	fscanf(in_file, "%s", magic_aux);
	im->n = 0;
	im->m = 0;
	im->max = 0;
	//apelam functia potrivita in functie de magic_aux
	if (strcmp(magic_aux, "P1") == 0 ||
		strcmp(magic_aux, "P2") == 0 || strcmp(magic_aux, "P3") == 0) {
		LOAD_ASCII(im, magic_aux, in_file, photo);
		printf("Loaded %s\n", file_name);
	} else if (strcmp(magic_aux, "P4") == 0 ||
		strcmp(magic_aux, "P5") == 0 || strcmp(magic_aux, "P6") == 0) {
		LOAD_BINARY(im, magic_aux, in_file, photo);
		printf("Loaded %s\n", file_name);
	}
	//selectam intreaga matricea la incarcarea fisierului
	im->x1 = 0;
	im->y1 = 0;
	im->x2 = im->n;
	im->y2 = im->m;
}

//functia SELECT selecteaza matericea pe axe pornind din colutul stanga sus
void SELECT(IMAGE *im, int photo)
{
	char line[100], all[100], x1[10], y1[10], x2[10], y2[10];
	char *token1;
	int X1, Y1, X2, Y2, aux = 0;
	//folosim functia fgets pentru a citi toata linia
	fgets(line, sizeof(line), stdin);
	if (photo == 0) {
		printf("No image loaded\n");
		return;
	}
	//folosim functia strtok pentru a separa cuvintele din linie
	token1 = strtok(line, " ");
	if (token1) {
		strncpy(all, token1, sizeof(all));
		all[strcspn(all, "\n")] = '\0'; //eliminam new line
	}
	//verificam daca primul cuvant este ALL pentru a decide tipul de selectie
	if (strcmp(all, "ALL") != 0) {
		//daca nu este all, punem coordonatele in token-uri
		strcpy(x1, all);
		token1 = strtok(NULL, " ");
		if (token1 != 0) {
			strncpy(y1, token1, sizeof(y1));
			y1[strlen(y1)] = '\0';
			token1 = strtok(NULL, " ");
			if (token1 != 0) {
				strncpy(x2, token1, sizeof(x2));
				x2[strlen(x2)] = '\0';
				token1 = strtok(NULL, " ");
				if (token1 != 0) {
					strncpy(y2, token1, sizeof(y2));
					y2[strlen(y2) - 1] = '\0';
				}
			}
		}
		X1 = ATOI_COORDINATES(x1);//facem coordonatele de tip int
		Y1 = ATOI_COORDINATES(y1);
		X2 = ATOI_COORDINATES(x2);
		Y2 = ATOI_COORDINATES(y2);
		if (X1 == -999 || Y1 == -999 || X2 == -999 || Y2 == -999) {
			printf("Invalid command\n");
			return;
		}
		if (X1 > X2) {
			aux = X1;
			X1 = X2;//verificam ordinea
			X2 = aux;
		}
		if (Y1 > Y2) {
			aux = Y1;
			Y1 = Y2;//verificam ordinea
			Y2 = aux;
		}
		if (X1 < 0 || X2 < 0 || Y1 < 0 || Y2 < 0 || X1 == X2 || Y1 == Y2 ||
			X1 > im->n || X2 > im->n || Y1 > im->m || Y2 > im->m) {
			printf("Invalid set of coordinates\n");
			return;
		}
		im->x1 = X1;
		im->y1 = Y1;
		im->x2 = X2;
		im->y2 = Y2;
		printf("Selected %d %d %d %d\n", X1, Y1, X2, Y2);
	} else {
		all[strlen(all) - 1] = '\0';
		im->x1 = 0;
		im->y1 = 0;
		im->x2 = im->n;
		im->y2 = im->m;
		printf("Selected ALL\n");
	}
}

void EQUALIZE(IMAGE *im, int photo)
{
	int i, j, k, H[260], e, s = 0, area = 0;

	if (photo == 0) {
		printf("No image loaded\n");
		return;
	}
	//verificam ca imaginea sa nu fie color
	if (strcmp(im->magic_word, "P3") == 0 ||
		strcmp(im->magic_word, "P6") == 0) {
		printf("Black and white image needed\n");
		return;
	}
	//calculam suprafata
	area = im->m * im->n;
	//initializam vectorul de frecventa cu 0
	for (i = 0; i < 256; i++)
		H[i] = 0;
	//construim vectorul de frecventa
	for (i = 0; i < im->m; i++)
		for (j = 0; j < im->n; j++) {
			int aux = im->matrix[i][j];
			H[aux]++;
		}

	for (i = 0; i < im->m; i++)
		for (j = 0; j < im->n; j++) {
			s = 0;
			//calculam suma conform formulei
			for (k = 0; k <= im->matrix[i][j]; k++)
				s += H[k];
			//pastram pixelii intre 0 si 255
			e = round((double)((255 * s) / area));
			CLAMP(e);
			//schimbam elementele matricei
			im->matrix[i][j] = e;
		}
	printf("Equalize done\n");
}

//functia CROP_MATRIX decupeaza matricele alb-negru si Grayscale
void CROP_MATRIX(IMAGE *im)
{
	int i, j;
	//alocam memorie pentru matricea decupata conform selectiei
	int **aux_matrix = malloc((im->y2 - im->y1) * sizeof(int *));
		if (!aux_matrix)
			return;
		for (i = 0; i < (im->y2 - im->y1); i++) {
			aux_matrix[i] = malloc((im->x2 - im->x1) * sizeof(int));
			if (!aux_matrix[i]) {
				for (j = 0; j < i; j++)
					free(aux_matrix[j]);
				free(aux_matrix);
				return;
			}
		}
		//punem elementele matricei originale in matricea auxiliara
		for (i = 0; i < (im->y2 - im->y1); i++)
			for (j = 0; j < (im->x2 - im->x1); j++)
				aux_matrix[i][j] = im->matrix[i + im->y1][j + im->x1];
		//eliberam matricea originala
		if (im->matrix) {
			for (i = 0; i < im->m; i++)
				free(im->matrix[i]);
			free(im->matrix);
		}
		//actualizam dimensiunile matricei cu celel ale selectiei
		im->m = im->y2 - im->y1;
		im->n = im->x2 - im->x1;
		//alocam din nou memoriei matricei folosind noile dimensiuni
		im->matrix = malloc(im->m * sizeof(int *));
		if (!im->matrix)
			return;
		for (i = 0; i < im->m; i++) {
			im->matrix[i] = malloc(im->n * sizeof(int));
			if (!im->matrix[i]) {
				for (j = 0; j < i; j++)
					free(im->matrix[j]);
				free(im->matrix);
				return;
			}
		}
		//egalam elementele cu cele din aux
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++)
				im->matrix[i][j] = aux_matrix[i][j];
		//eliberam aux
		for (i = 0; i < im->m; i++)
			free(aux_matrix[i]);
		free(aux_matrix);
		//selectam intreaga matrice
		im->x1 = 0;
		im->y1 = 0;
		im->x2 = im->n;
		im->y2 = im->m;
		printf("Image cropped\n");
}

//functia CROP_RGB decupeaza matricele rgb, in acelasi fel prezentat mai sus
void CROP_RGB(IMAGE *im)
{
	int i, j;
	RGB_MATRIX **aux_rgb = malloc((im->y2 - im->y1) * sizeof(RGB_MATRIX *));
		if (!aux_rgb)
			return;
		for (i = 0; i < (im->y2 - im->y1); i++) {
			aux_rgb[i] = malloc((im->x2 - im->x1) * sizeof(RGB_MATRIX));
			if (!aux_rgb[i]) {
				for (j = 0; j < i; j++)
					free(aux_rgb[j]);
				free(aux_rgb);
				return;
			}
		}
		for (i = 0; i < (im->y2 - im->y1); i++)
			for (j = 0; j < (im->x2 - im->x1); j++) {
				aux_rgb[i][j].r = im->rgb[i + im->y1][j + im->x1].r;
				aux_rgb[i][j].g = im->rgb[i + im->y1][j + im->x1].g;
				aux_rgb[i][j].b = im->rgb[i + im->y1][j + im->x1].b;
			}
		if (im->rgb) {
			for (i = 0; i < im->m; i++)
				free(im->rgb[i]);
			free(im->rgb);
		}
		im->m = im->y2 - im->y1;
		im->n = im->x2 - im->x1;
		im->rgb = malloc(im->m * sizeof(RGB_MATRIX *));
		if (!im->rgb)
			return;
		for (i = 0; i < im->m; i++) {
			im->rgb[i] = malloc(im->n * sizeof(RGB_MATRIX));
			if (!im->rgb[i]) {
				for (j = 0; j < i; j++)
					free(im->rgb[j]);
				free(im->rgb);
				return;
			}
		}
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++) {
				im->rgb[i][j].r = aux_rgb[i][j].r;
				im->rgb[i][j].g = aux_rgb[i][j].g;
				im->rgb[i][j].b = aux_rgb[i][j].b;
			}
		for (i = 0; i < im->m; i++)
			free(aux_rgb[i]);
		free(aux_rgb);
		im->x1 = 0;
		im->y1 = 0;
		im->x2 = im->n;
		im->y2 = im->m;
		printf("Image cropped\n");
}

//functia CROP decupeaza imaginile in functie de tip
void CROP(IMAGE *im, int photo)
{
	if (photo == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(im->magic_word, "P3") == 0 ||
		strcmp(im->magic_word, "P6") == 0) {
		CROP_RGB(im);
	} else {
		CROP_MATRIX(im);
	}
}

void APPLY_EDGE(IMAGE *im)
{
	int i, j;
	RGB_MATRIX **aux = malloc((im->y2 - im->y1) * sizeof(RGB_MATRIX *));
	if (!aux)
		return;
	for (i = 0; i < (im->y2 - im->y1); i++) {
		aux[i] = malloc((im->x2 - im->x1) * sizeof(RGB_MATRIX));
		if (!aux[i]) {
			for (j = 0; j < i; j++)
				free(aux[j]);
			free(aux);
			return;
		}
	}
	/*parcurgem matricea selectata si calculam elemetele din aux,
	care are aceleasi dimensiuni cu selectia, folosind vecinii elementelor*/
	for (i = 0; i < (im->y2 - im->y1); i++)
		for (j = 0; j < (im->x2 - im->x1); j++) {
			/*evitam schibarea marginilor matricei, elementele de pe acestea,
			neavand vecini*/
			if (i + im->y1 == 0 || i + im->y1 == im->m - 1 ||
				j + im->x1 == 0 || j + im->x1 == im->n - 1) {
				aux[i][j].r = im->rgb[i + im->y1][j + im->x1].r;
				aux[i][j].g = im->rgb[i + im->y1][j + im->x1].g;
				aux[i][j].b = im->rgb[i + im->y1][j + im->x1].b;
			} else {
				aux[i][j].r = 0;
				aux[i][j].r += im->rgb[i - 1 + im->y1][j - 1 + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i - 1 + im->y1][j + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i - 1 + im->y1][j + 1 + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i + im->y1][j - 1 + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i + im->y1][j + im->x1].r * 8;
				aux[i][j].r += im->rgb[i + im->y1][j + 1 + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i + 1 + im->y1][j - 1 + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i + 1 + im->y1][j + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i + 1 + im->y1][j + 1 + im->x1].r * (-1);

				aux[i][j].g = 0;
				aux[i][j].g += im->rgb[i - 1 + im->y1][j - 1 + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i - 1 + im->y1][j + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i - 1 + im->y1][j + 1 + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i + im->y1][j - 1 + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i + im->y1][j + im->x1].g * 8;
				aux[i][j].g += im->rgb[i + im->y1][j + 1 + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i + 1 + im->y1][j - 1 + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i + 1 + im->y1][j + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i + 1 + im->y1][j + 1 + im->x1].g * (-1);

				aux[i][j].b = 0;
				aux[i][j].b += im->rgb[i - 1 + im->y1][j - 1 + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i - 1 + im->y1][j + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i - 1 + im->y1][j + 1 + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i + im->y1][j - 1 + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i + im->y1][j + im->x1].b * 8;
				aux[i][j].b += im->rgb[i + im->y1][j + 1 + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i + 1 + im->y1][j - 1 + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i + 1 + im->y1][j + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i + 1 + im->y1][j + 1 + im->x1].b * (-1);
			}
		}
	for (i = 0; i < (im->y2 - im->y1); i++)
		for	(j = 0; j < (im->x2 - im->x1); j++) {
			//pastram valorile intre 0 si 255
			aux[i][j].r = CLAMP(aux[i][j].r);
			aux[i][j].g = CLAMP(aux[i][j].g);
			aux[i][j].b = CLAMP(aux[i][j].b);
			aux[i][j].r = round((double)aux[i][j].r);
			aux[i][j].b = round((double)aux[i][j].b);
			aux[i][j].g = round((double)aux[i][j].g);
			//adaugam schimbarile in matrice
			im->rgb[i + im->y1][j + im->x1].r = aux[i][j].r;
			im->rgb[i + im->y1][j + im->x1].g = aux[i][j].g;
			im->rgb[i + im->y1][j + im->x1].b = aux[i][j].b;
		}
	for (i = 0; i < (im->y2 - im->y1); i++)
		free(aux[i]);
	free(aux);
	printf("APPLY EDGE done\n");
}

/*functia APPLY_SHARPEN foloseste procedeul prezentat in functia APPLY_EDGE,
insa innmulteste vecinii cu alte valori conform efectului dorit*/
void APPLY_SHARPEN(IMAGE *im)
{
	int i, j;
	RGB_MATRIX **aux = malloc((im->y2 - im->y1) * sizeof(RGB_MATRIX *));
	if (!aux)
		return;
	for	(i = 0; i < (im->y2 - im->y1); i++) {
		aux[i] = malloc((im->x2 - im->x1) * sizeof(RGB_MATRIX));
		if	(!aux[i]) {
			for (j = 0; j < i; j++)
				free(aux[j]);
			free(aux);
			return;
		}
	}
	for (i = 0; i < (im->y2 - im->y1); i++)
		for	(j = 0; j < (im->x2 - im->x1); j++) {
			if (i + im->y1 == 0 || i + im->y1 == im->m - 1 ||
				j + im->x1 == 0 || j + im->x1 == im->n - 1) {
				aux[i][j].r = im->rgb[i + im->y1][j + im->x1].r;
				aux[i][j].g = im->rgb[i + im->y1][j + im->x1].g;
				aux[i][j].b = im->rgb[i + im->y1][j + im->x1].b;
			} else {
				aux[i][j].r = 0;
				aux[i][j].r += im->rgb[i - 1 + im->y1][j - 1 + im->x1].r * 0;
				aux[i][j].r += im->rgb[i - 1 + im->y1][j + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i - 1 + im->y1][j + 1 + im->x1].r * 0;
				aux[i][j].r += im->rgb[i + im->y1][j - 1 + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i + im->y1][j + im->x1].r * 5;
				aux[i][j].r += im->rgb[i + im->y1][j + 1 + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i + 1 + im->y1][j - 1 + im->x1].r * 0;
				aux[i][j].r += im->rgb[i + 1 + im->y1][j + im->x1].r * (-1);
				aux[i][j].r += im->rgb[i + 1 + im->y1][j + 1 + im->x1].r * 0;

				aux[i][j].g = 0;
				aux[i][j].g += im->rgb[i - 1 + im->y1][j - 1 + im->x1].g * 0;
				aux[i][j].g += im->rgb[i - 1 + im->y1][j + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i - 1 + im->y1][j + 1 + im->x1].g * 0;
				aux[i][j].g += im->rgb[i + im->y1][j - 1 + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i + im->y1][j + im->x1].g * 5;
				aux[i][j].g += im->rgb[i + im->y1][j + 1 + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i + 1 + im->y1][j - 1 + im->x1].g * 0;
				aux[i][j].g += im->rgb[i + 1 + im->y1][j + im->x1].g * (-1);
				aux[i][j].g += im->rgb[i + 1 + im->y1][j + 1 + im->x1].g * 0;

				aux[i][j].b = 0;
				aux[i][j].b += im->rgb[i - 1 + im->y1][j - 1 + im->x1].b * 0;
				aux[i][j].b += im->rgb[i - 1 + im->y1][j + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i - 1 + im->y1][j + 1 + im->x1].b * 0;
				aux[i][j].b += im->rgb[i + im->y1][j - 1 + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i + im->y1][j + im->x1].b * 5;
				aux[i][j].b += im->rgb[i + im->y1][j + 1 + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i + 1 + im->y1][j - 1 + im->x1].b * 0;
				aux[i][j].b += im->rgb[i + 1 + im->y1][j + im->x1].b * (-1);
				aux[i][j].b += im->rgb[i + 1 + im->y1][j + 1 + im->x1].b * 0;
			}
		}
	for (i = 0; i < (im->y2 - im->y1); i++)
		for	(j = 0; j < (im->x2 - im->x1); j++) {
			aux[i][j].r = CLAMP(aux[i][j].r);
			aux[i][j].g = CLAMP(aux[i][j].g);
			aux[i][j].b = CLAMP(aux[i][j].b);
			aux[i][j].r = round((double)aux[i][j].r);
			aux[i][j].b = round((double)aux[i][j].b);
			aux[i][j].g = round((double)aux[i][j].g);
			im->rgb[i + im->y1][j + im->x1].r = aux[i][j].r;
			im->rgb[i + im->y1][j + im->x1].g = aux[i][j].g;
			im->rgb[i + im->y1][j + im->x1].b = aux[i][j].b;
		}

	for (i = 0; i < (im->y2 - im->y1); i++)
		free(aux[i]);
	free(aux);
	printf("APPLY SHARPEN done\n");
}

/*functia APPLY_BLUR foloseste procedeul prezentat in functia APPLY_EDGE,
insa innmulteste vecinii cu alte valori conform efectului dorit*/
void APPLY_BLUR(IMAGE *im)
{
	int i, j;
	RGB_MATRIX **aux = malloc((im->y2 - im->y1) * sizeof(RGB_MATRIX *));
	if (!aux)
		return;
	for	(i = 0; i < (im->y2 - im->y1); i++) {
		aux[i] = malloc((im->x2 - im->x1) * sizeof(RGB_MATRIX));
		if	(!aux[i]) {
			for (j = 0; j < i; j++)
				free(aux[j]);
			free(aux);
			return;
		}
	}
	for (i = 0; i < (im->y2 - im->y1); i++)
		for	(j = 0; j < (im->x2 - im->x1); j++) {
			if (i + im->y1 == 0 || i + im->y1 == im->m - 1 ||
				j + im->x1 == 0 || j + im->x1 == im->n - 1) {
				aux[i][j].r = im->rgb[i + im->y1][j + im->x1].r;
				aux[i][j].g = im->rgb[i + im->y1][j + im->x1].g;
				aux[i][j].b = im->rgb[i + im->y1][j + im->x1].b;
			} else {
				aux[i][j].r = 0;
				aux[i][j].r += im->rgb[i - 1 + im->y1][j - 1 + im->x1].r;
				aux[i][j].r += im->rgb[i - 1 + im->y1][j + im->x1].r;
				aux[i][j].r += im->rgb[i - 1 + im->y1][j + 1 + im->x1].r;
				aux[i][j].r += im->rgb[i + im->y1][j - 1 + im->x1].r;
				aux[i][j].r += im->rgb[i + im->y1][j + im->x1].r;
				aux[i][j].r += im->rgb[i + im->y1][j + 1 + im->x1].r;
				aux[i][j].r += im->rgb[i + 1 + im->y1][j - 1 + im->x1].r;
				aux[i][j].r += im->rgb[i + 1 + im->y1][j + im->x1].r;
				aux[i][j].r += im->rgb[i + 1 + im->y1][j + 1 + im->x1].r;

				aux[i][j].g = 0;
				aux[i][j].g += im->rgb[i - 1 + im->y1][j - 1 + im->x1].g;
				aux[i][j].g += im->rgb[i - 1 + im->y1][j + im->x1].g;
				aux[i][j].g += im->rgb[i - 1 + im->y1][j + 1 + im->x1].g;
				aux[i][j].g += im->rgb[i + im->y1][j - 1 + im->x1].g;
				aux[i][j].g += im->rgb[i + im->y1][j + im->x1].g;
				aux[i][j].g += im->rgb[i + im->y1][j + 1 + im->x1].g;
				aux[i][j].g += im->rgb[i + 1 + im->y1][j - 1 + im->x1].g;
				aux[i][j].g += im->rgb[i + 1 + im->y1][j + im->x1].g;
				aux[i][j].g += im->rgb[i + 1 + im->y1][j + 1 + im->x1].g;

				aux[i][j].b = 0;
				aux[i][j].b += im->rgb[i - 1 + im->y1][j - 1 + im->x1].b;
				aux[i][j].b += im->rgb[i - 1 + im->y1][j + im->x1].b;
				aux[i][j].b += im->rgb[i - 1 + im->y1][j + 1 + im->x1].b;
				aux[i][j].b += im->rgb[i + im->y1][j - 1 + im->x1].b;
				aux[i][j].b += im->rgb[i + im->y1][j + im->x1].b;
				aux[i][j].b += im->rgb[i + im->y1][j + 1 + im->x1].b;
				aux[i][j].b += im->rgb[i + 1 + im->y1][j - 1 + im->x1].b;
				aux[i][j].b += im->rgb[i + 1 + im->y1][j + im->x1].b;
				aux[i][j].b += im->rgb[i + 1 + im->y1][j + 1 + im->x1].b;

				aux[i][j].r = round((double)aux[i][j].r / 9);
				aux[i][j].g = round((double)aux[i][j].g / 9);
				aux[i][j].b = round((double)aux[i][j].b / 9);
				CLAMP(aux[i][j].r);
				CLAMP(aux[i][j].g);
				CLAMP(aux[i][j].b);
			}
		}
	for (i = 0; i < (im->y2 - im->y1); i++)
		for (j = 0; j < (im->x2 - im->x1); j++) {
			im->rgb[i + im->y1][j + im->x1].r = aux[i][j].r;
			im->rgb[i + im->y1][j + im->x1].g = aux[i][j].g;
			im->rgb[i + im->y1][j + im->x1].b = aux[i][j].b;
		}
	for (i = 0; i < (im->y2 - im->y1); i++)
		free(aux[i]);
	free(aux);
	printf("APPLY BLUR done\n");
}

/*functia APPLY_GAUSSIAN_BLUR foloseste procedeul prezentat in functia
APPLY_EDGE, insa innmulteste vecinii cu alte valori conform efectului dorit*/
void APPLY_GAUSSIAN_BLUR(IMAGE *im)
{
	int i, j;
	RGB_MATRIX **aux = malloc((im->y2 - im->y1) * sizeof(RGB_MATRIX *));
	if (!aux)
		return;
	for	(i = 0; i < (im->y2 - im->y1); i++) {
		aux[i] = malloc((im->x2 - im->x1) * sizeof(RGB_MATRIX));
		if	(!aux[i]) {
			for (j = 0; j < i; j++)
				free(aux[j]);
			free(aux);
			return;
		}
	}
	for (i = 0; i < (im->y2 - im->y1); i++)
		for	(j = 0; j < (im->x2 - im->x1); j++) {
			if (i + im->y1 == 0 || i + im->y1 == im->m - 1 ||
				j + im->x1 == 0 || j + im->x1 == im->n - 1) {
				aux[i][j].r = im->rgb[i + im->y1][j + im->x1].r;
				aux[i][j].g = im->rgb[i + im->y1][j + im->x1].g;
				aux[i][j].b = im->rgb[i + im->y1][j + im->x1].b;
			} else {
				aux[i][j].r = 0;
				aux[i][j].r += im->rgb[i - 1 + im->y1][j - 1 + im->x1].r;
				aux[i][j].r += im->rgb[i - 1 + im->y1][j + im->x1].r * 2;
				aux[i][j].r += im->rgb[i - 1 + im->y1][j + 1 + im->x1].r;
				aux[i][j].r += im->rgb[i + im->y1][j - 1 + im->x1].r * 2;
				aux[i][j].r += im->rgb[i + im->y1][j + im->x1].r * 4;
				aux[i][j].r += im->rgb[i + im->y1][j + 1 + im->x1].r * 2;
				aux[i][j].r += im->rgb[i + 1 + im->y1][j - 1 + im->x1].r;
				aux[i][j].r += im->rgb[i + 1 + im->y1][j + im->x1].r * 2;
				aux[i][j].r += im->rgb[i + 1 + im->y1][j + 1 + im->x1].r;

				aux[i][j].g = 0;
				aux[i][j].g += im->rgb[i - 1 + im->y1][j - 1 + im->x1].g;
				aux[i][j].g += im->rgb[i - 1 + im->y1][j + im->x1].g * 2;
				aux[i][j].g += im->rgb[i - 1 + im->y1][j + 1 + im->x1].g;
				aux[i][j].g += im->rgb[i + im->y1][j - 1 + im->x1].g * 2;
				aux[i][j].g += im->rgb[i + im->y1][j + im->x1].g * 4;
				aux[i][j].g += im->rgb[i + im->y1][j + 1 + im->x1].g * 2;
				aux[i][j].g += im->rgb[i + 1 + im->y1][j - 1 + im->x1].g;
				aux[i][j].g += im->rgb[i + 1 + im->y1][j + im->x1].g * 2;
				aux[i][j].g += im->rgb[i + 1 + im->y1][j + 1 + im->x1].g;

				aux[i][j].b = 0;
				aux[i][j].b += im->rgb[i - 1 + im->y1][j - 1 + im->x1].b;
				aux[i][j].b += im->rgb[i - 1 + im->y1][j + im->x1].b * 2;
				aux[i][j].b += im->rgb[i - 1 + im->y1][j + 1 + im->x1].b;
				aux[i][j].b += im->rgb[i + im->y1][j - 1 + im->x1].b * 2;
				aux[i][j].b += im->rgb[i + im->y1][j + im->x1].b * 4;
				aux[i][j].b += im->rgb[i + im->y1][j + 1 + im->x1].b * 2;
				aux[i][j].b += im->rgb[i + 1 + im->y1][j - 1 + im->x1].b;
				aux[i][j].b += im->rgb[i + 1 + im->y1][j + im->x1].b * 2;
				aux[i][j].b += im->rgb[i + 1 + im->y1][j + 1 + im->x1].b;

				aux[i][j].r = round((double)aux[i][j].r / 16);
				aux[i][j].g = round((double)aux[i][j].g / 16);
				aux[i][j].b = round((double)aux[i][j].b / 16);
				CLAMP(aux[i][j].r);
				CLAMP(aux[i][j].g);
				CLAMP(aux[i][j].b);
			}
		}
	for (i = 0; i < (im->y2 - im->y1); i++)
		for	(j = 0; j < (im->x2 - im->x1); j++) {
			im->rgb[i + im->y1][j + im->x1].r = aux[i][j].r;
			im->rgb[i + im->y1][j + im->x1].g = aux[i][j].g;
			im->rgb[i + im->y1][j + im->x1].b = aux[i][j].b;
		}

	for (i = 0; i < (im->y2 - im->y1); i++)
		free(aux[i]);
	free(aux);
	printf("APPLY GAUSSIAN_BLUR done\n");
}

void APPLY(IMAGE *im, int photo)
{
	char line[100], parameter[50];
	char *token2;
	//citim toata linia
	fgets(line, sizeof(line), stdin);
	if (photo == 0) {
		printf("No image loaded\n");
		return;
	}
	//folosim strtok pentru a obtine parametrul comenzii
	token2 = strtok(line, " ");
	if (token2) {
		strncpy(parameter, token2, sizeof(parameter));
		parameter[strcspn(parameter, "\n")] = '\0';
	}
	if (strcmp(parameter, "") == 0) {
		printf("Invalid command\n");
		return;
	}
	if (strcmp(parameter, "EDGE") == 0) {
		if (strcmp(im->magic_word, "P3") != 0 &&
			strcmp(im->magic_word, "P6") != 0) {
			printf("Easy, Charlie Chaplin\n");
			return;
		}
		APPLY_EDGE(im);
	} else if (strcmp(parameter, "SHARPEN") == 0) {
		if (strcmp(im->magic_word, "P3") != 0 &&
			strcmp(im->magic_word, "P6") != 0) {
			printf("Easy, Charlie Chaplin\n");
			return;
		}
		APPLY_SHARPEN(im);
	} else if (strcmp(parameter, "BLUR") == 0) {
		if (strcmp(im->magic_word, "P3") != 0 &&
			strcmp(im->magic_word, "P6") != 0) {
			printf("Easy, Charlie Chaplin\n");
			return;
		}
		APPLY_BLUR(im);
	} else if (strcmp(parameter, "GAUSSIAN_BLUR") == 0) {
		if (strcmp(im->magic_word, "P3") != 0 &&
			strcmp(im->magic_word, "P6") != 0) {
			printf("Easy, Charlie Chaplin\n");
			return;
		}
		APPLY_GAUSSIAN_BLUR(im);
	} else {
		printf("APPLY parameter invalid\n");
		return;
	}
}

void SAVE_ASCII(IMAGE *im, FILE *out_file)
{
	int i, j;
	/*printam matricele in functie de tipul imaginii, adica in functie de
	magic_word*/
	if (strcmp(im->magic_word, "P1") == 0) {
		//schimbam magic_word in functie de tipl in care se salveaza
		fprintf(out_file, "P1\n");
		fprintf(out_file, "%d %d\n", im->n, im->m);
		for (i = 0; i < im->m; i++) {
			for (j = 0; j < im->n; j++)
				fprintf(out_file, "%d ", im->matrix[i][j]);
			fprintf(out_file, "\n");
		}
	}
	if (strcmp(im->magic_word, "P4") == 0) {
		fprintf(out_file, "P1\n");
		fprintf(out_file, "%d %d\n", im->n, im->m);
		for (i = 0; i < im->m; i++) {
			for (j = 0; j < im->n; j++)
				fprintf(out_file, "%d ", im->matrix[i][j]);
			fprintf(out_file, "\n");
		}
	}
	if (strcmp(im->magic_word, "P2") == 0)
		fprintf(out_file, "P2\n");
	if (strcmp(im->magic_word, "P5") == 0)
		fprintf(out_file, "P2\n");
	if (strcmp(im->magic_word, "P2") == 0 ||
		strcmp(im->magic_word, "P5") == 0) {
		fprintf(out_file, "%d %d\n", im->n, im->m);
		fprintf(out_file, "%d\n", im->max);
		for (i = 0; i < im->m; i++) {
			for (j = 0; j < im->n; j++)
				fprintf(out_file, "%d ", im->matrix[i][j]);
			fprintf(out_file, "\n");
		}
	}
	if (strcmp(im->magic_word, "P3") == 0)
		fprintf(out_file, "P3\n");
	if (strcmp(im->magic_word, "P6") == 0)
		fprintf(out_file, "P3\n");
	if (strcmp(im->magic_word, "P3") == 0 ||
		strcmp(im->magic_word, "P6") == 0) {
		fprintf(out_file, "%d %d\n", im->n, im->m);
		fprintf(out_file, "%d\n", im->max);
		for (i = 0; i < im->m; i++) {
			for (j = 0; j < im->n; j++) {
				fprintf(out_file, "%d ", im->rgb[i][j].r);
				fprintf(out_file, "%d ", im->rgb[i][j].g);
				fprintf(out_file, "%d ", im->rgb[i][j].b);
			}
			fprintf(out_file, "\n");
		}
	}
	fclose(out_file);
}

void SAVE_BINARY(IMAGE *im, FILE *out_file)
{
	int i, j;
	/*printam matricele in functie de tipul imaginii, adica in functie de
	magic_word*/
	if (strcmp(im->magic_word, "P1") == 0) {
		//schimbam magic_word in functie de tipl in care se salveaza
		fprintf(out_file, "P4\n");
		fprintf(out_file, "%d %d\n", im->n, im->m);
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++)
			//folosim fwrite pentru tipul binar
				fwrite(&im->matrix[i][j], sizeof(char), 1, out_file);
	}
	if (strcmp(im->magic_word, "P4") == 0) {
		fprintf(out_file, "P4\n");
		fprintf(out_file, "%d %d\n", im->n, im->m);
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++)
				fwrite(&im->matrix[i][j], sizeof(char), 1, out_file);
	}
	if (strcmp(im->magic_word, "P2") == 0)
		fprintf(out_file, "P5\n");
	if (strcmp(im->magic_word, "P5") == 0)
		fprintf(out_file, "P5\n");
	if (strcmp(im->magic_word, "P2") == 0 ||
		strcmp(im->magic_word, "P5") == 0) {
		fprintf(out_file, "%d %d\n", im->n, im->m);
		fprintf(out_file, "%d\n", im->max);
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++)
				fwrite(&im->matrix[i][j], sizeof(char), 1, out_file);
	}
	if (strcmp(im->magic_word, "P3") == 0)
		fprintf(out_file, "P6\n");
	if (strcmp(im->magic_word, "P6") == 0)
		fprintf(out_file, "P6\n");
	if (strcmp(im->magic_word, "P3") == 0 ||
		strcmp(im->magic_word, "P6") == 0) {
		fprintf(out_file, "%d %d\n", im->n, im->m);
		fprintf(out_file, "%d\n", im->max);
		for (i = 0; i < im->m; i++)
			for (j = 0; j < im->n; j++) {
				fwrite(&im->rgb[i][j].r, sizeof(char), 1, out_file);
				fwrite(&im->rgb[i][j].g, sizeof(char), 1, out_file);
				fwrite(&im->rgb[i][j].b, sizeof(char), 1, out_file);
			}
	}
	fclose(out_file);
}

//functia SAVE salveaza in fisier imaginile in ASCII sau binar
void SAVE(IMAGE *im, int photo)
{
	char line[100], file_name[1000], file_type[100];
	char *token;
	//citim intreaga linie
	fgets(line, sizeof(line), stdin);
	if (photo == 0) {
		printf("No image loaded\n");
		return;
	}
	file_type[0] = '\0';
	//folosim strtok pentru a obtine tipul in care salvam
	token = strtok(line, " ");
	if (token) {
		strncpy(file_name, token, sizeof(file_name));
		//obtinem numele fisierului
		file_name[strcspn(file_name, "\n")] = '\0';
		token = strtok(NULL, " ");
		//obtinem tipul in care va salva
		if (token) {
			strncpy(file_type, token, sizeof(file_type));
			file_type[strlen(file_type) - 1] = '\0';
		}
	}
	//daca se salveaza ascii deschidem fisierul cu wt, iar binar cu wb
	if (strcmp(file_type, "ascii") == 0) {
		FILE *out_file = fopen(file_name, "wt");
		if (!out_file)
			return;
		SAVE_ASCII(im, out_file);
		printf("Saved %s\n", file_name);
	} else {
		FILE *out_file = fopen(file_name, "wb");
		if (!out_file)
			return;
		SAVE_BINARY(im, out_file);
		printf("Saved %s\n", file_name);
	}
}

//functia EXIT elibereaza resursele
void EXIT(IMAGE *im, int photo)
{
	int i;
	if (photo == 0) {
		printf("No image loaded\n");
		return;
	}
	//eliberam matricea normala sau matricea rgb in functie de caz
	if (im->matrix) {
		for (i = 0; i < im->m; i++)
			free(im->matrix[i]);
		free(im->matrix);
		im->matrix = NULL;
	} else if (im->rgb) {
		for (i = 0; i < im->m; i++)
			free(im->rgb[i]);
		free(im->rgb);
		im->rgb = NULL;
	}
}

int main(void)
{
	char command[100];
	int photo = 0;
	//citim de la tastatura comanda
	scanf("%s", command);
	//declaram structura
	IMAGE im;
	im.matrix = NULL;
	im.rgb = NULL;
	while (strcmp(command, "EXIT") != 0) {
		if (strcmp(command, "LOAD") == 0) {
			LOAD(&im, &photo);
		} else if (strcmp(command, "SELECT") == 0) {
			SELECT(&im, photo);
		} else if (strcmp(command, "EQUALIZE") == 0) {
			EQUALIZE(&im, photo);
		} else if (strcmp(command, "CROP") == 0) {
			CROP(&im, photo);
		} else if (strcmp(command, "APPLY") == 0) {
			APPLY(&im, photo);
		} else if (strcmp(command, "SAVE") == 0) {
			SAVE(&im, photo);
		} else {
			fgets(command, 100, stdin);
			printf("Invalid command\n");
		}
		//citim o noua coamnda
		scanf("%s", command);
	}
	EXIT(&im, photo);
	return 0;
}
