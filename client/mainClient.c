#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "struct.h"
#include "client.h"


int main( int argc, char *argv[] ) {
	
	char *message;
	struct reponse_serveur rq;
	int nbr_para = 4;
	int quitter = 0;
	int choix, req;


	if( argc != 4 ) {
      printf("Pas assez d'argument\n");
	  printf("./exe_client_ad login password ip_serveur\n");
	  return 1;
   }


	if(Initialisation(argv[3]) != 1) {
		printf("Erreur d'initialisation\n");
		return 1;
	}

	AffichageMenu(argv[1]);
	while(quitter != 1)
	{
		// Lecture du choix de l'utilisateur + vérification de saisie
		while(scanf("%d", &choix) != 1)
		{
			// vidage du buffer clavier
			while ((getchar()) != '\n');
			printf("Veuillez saisir un nombre entier :\n");
		}
		if (strcmp(argv[1], "admin") == 0)
		{
			switch(choix)
			{
				case 1:
					req = EnvoiRequeteGet(argv[1], argv[2]);
					AffichageMenu(argv[1]);
					break;
				case 2:
					req = EnvoiRequeteCreate(argv[1], argv[2]);
					AffichageMenu(argv[1]);
					break;
				case 3:
					req = EnvoiRequeteUpdate(argv[1], argv[2]);
					AffichageMenu(argv[1]);
					break;
				case 4:
					req = EnvoiRequeteDelete(argv[1], argv[2]);
					AffichageMenu(argv[1]);
					break;
				case 5:
					puts("/!\\Zone en travaux /!\\\n");
					sleep(5);
					AffichageMenu(argv[1]);
					break;
				case 6:
					quitter = 1;
					break;
				default:
					printf("Entrez un chiffre entre 1 et 6\n");
					break;
			}	
		}
		else
		{
			switch(choix)
			{
				case 1:
					req = EnvoiRequeteGet(argv[1], argv[2]);
					AffichageMenu(argv[1]);
					break;
				case 2:
					req = EnvoiRequeteCreate(argv[1], argv[2]);
					AffichageMenu(argv[1]);
					break;
				case 3:
					req = EnvoiRequeteUpdate(argv[1], argv[2]);
					AffichageMenu(argv[1]);
					break;
				case 4:
					req = EnvoiRequeteDelete(argv[1], argv[2]);
					AffichageMenu(argv[1]);
					break;
				case 5:
					quitter = 1;
					break;
				default:
					printf("Entrez un chiffre entre 1 et 5\n");
					break;
			}
		}
	}

	Terminaison();

	return 0;
}
