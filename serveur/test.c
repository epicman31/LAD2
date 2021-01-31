#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// lecture du fichier filename dans contenu de longueur taille
int lecture(char *filename, char *contenu, int taille)
{
    FILE *fp;
	
	if((fp = fopen(filename, "r")) == NULL) { // ouverture fichier
		return(-1);
	}

	fread(contenu, taille, 1, fp); //lecture du fichier dans contenu
	fclose(fp);
	return(0);
}
// Fonction de calcul de taille d'un fichier
unsigned long longueur_fichier(char *file_name)
{
	FILE *file;

    /* INITIALISATION DES PARAMETRES */

	unsigned long fin = 0;

	// OUVERTURE DU FICHIER

	if ((file = fopen(file_name, "r")) == NULL)
	{
		fputs("Impossible d ouvrir le fichier.", stdout);
	}
	else
	{
		fseek(file,0,SEEK_END);
		fin = ftell(file); //fin du fichier
	}

	fclose(file);

	return fin;
}

int check_credentials(char *login, char *password)
{
	unsigned long taille = 0; // initialisation taille

	char filename[] = "admin-permission.txt"; // nom fichier
	taille = longueur_fichier(filename); // renvoie la longueur du fichier

	char * contenu;	
	contenu = (char *) malloc(taille); // variable de la taille du fichier
	lecture(filename, contenu, taille);

	if(strstr(contenu, login) != NULL)
	{
		char *var = strstr(contenu, login);
		int position = var - contenu; // position de la première lettre du login
		int min_pwd, max_pwd, lg_pwd = 0; // variable de position du début et de la fin et de longueur du password

		while(1)
		{
			if (contenu[position] == ',')
			{
				min_pwd = position;
			}
			if (contenu[position] == ';')
			{
				max_pwd = position;
				break;
			}
			++position;
		}
		lg_pwd = max_pwd - min_pwd; // calcul de la longueur pour init la variable test_pswd 
		printf("%d\n", lg_pwd);
		char test_pswd[lg_pwd];

		for (int i = min_pwd+1; i < max_pwd; ++i)
		{

			test_pswd[i-min_pwd-1] = contenu[i];
			if (i == (max_pwd-1))
			{
				test_pswd[lg_pwd-1] = '\0';
				break;
			}
			//printf("%c\n", test_pswd[i-min_pwd]);
		}

		if (strcmp(test_pswd, password) == 0)
		{
			return 0;
		}
		else return 1 ;
		
		
	}
	else {return 1 ;}
	free(contenu);
}

void main(void)
{
	char test_pswd[] = "pasword";
	char test_login[] = "admin";
	int result;
	result = check_credentials(test_login,test_pswd);
	if (result == 0)
	{
		printf("OK !\n");
	}
	else{
		printf("Authentification failed\n");
	}

	
}