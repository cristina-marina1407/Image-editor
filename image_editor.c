//Postelnicu Cristina-Marina 313CA 2023-2024
#include "helper.h"
#include "functions.h"

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
