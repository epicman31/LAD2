
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "struct.h"
#include "client.h"

#define TRUE 1
#define FALSE 0

#define LONGUEUR_TAMPON 4096
#define TAILLE_PDU 250

/* Variables cachees */

/* le socket client */
int socketClient;
/* le tampon de reception */
char tamponClient[LONGUEUR_TAMPON];
int debutTampon;
int finTampon;




/* Initialisation.
 * Connexion au serveur sur la machine donnee.
 * Utilisez localhost pour un fonctionnement local.
 */
int Initialisation(char *machine) {
	return InitialisationAvecService(machine, "13214");
}

/* Initialisation.
 * Connexion au serveur sur la machine donnee et au service donne.
 * Utilisez localhost pour un fonctionnement local.
 */
int InitialisationAvecService(char *machine, char *service) {
	int n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ( (n = getaddrinfo(machine, service, &hints, &res)) != 0)  {
     		fprintf(stderr, "Initialisation, erreur de getaddrinfo : %s", gai_strerror(n));
     		return 0;
	}
	ressave = res;

	do {
		socketClient = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketClient < 0)
			continue;	/* ignore this one */

		if (connect(socketClient, res->ai_addr, res->ai_addrlen) == 0)
			break;		/* success */

		close(socketClient);	/* ignore this one */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL) {
     		perror("Initialisation, erreur de connect.");
     		return 0;
	}

	freeaddrinfo(ressave);
	printf("Connexion avec le serveur reussie.\n");

	return 1;
}

/* Recoit un message envoye par le serveur.
 */
char *Reception() {
	int retour = 0;
	memset(tamponClient,0,LONGUEUR_TAMPON);
	retour = recv(socketClient, tamponClient, LONGUEUR_TAMPON-1, 0);
	if (retour < 0) {
		perror("Reception, erreur de recv.");
		return NULL;
	} else if(retour == 0) {
		fprintf(stderr, "Reception, le serveur a ferme la connexion.\n");
		return NULL;
	} else {
		return strdup(tamponClient);
	}
}


/* Envoie un message au serveur.
 * Attention, le message doit etre termine par \n
 */
int Emission(char *message) {
	if(strstr(message, "\n") == NULL) {
		fprintf(stderr, "Emission, Le message n'est pas termine par \\n.\n");
	}
	int taille = strlen(message);
	if (send(socketClient, message, taille,0) == -1) {
        perror("Emission, probleme lors du send.");
        return 0;
	}
	printf("Emission de %d caracteres.\n", taille+1);
	return 1;
}

/* Recoit des donnees envoyees par le serveur.
 */
int ReceptionBinaire(char *donnees, size_t tailleMax) {
	int dejaRecu = 0;
	int retour = 0;
	/* on commence par recopier tout ce qui reste dans le tampon
	 */
	while((finTampon > debutTampon) && (dejaRecu < tailleMax)) {
		donnees[dejaRecu] = tamponClient[debutTampon];
		dejaRecu++;
		debutTampon++;
	}
	/* si on n'est pas arrive au max
	 * on essaie de recevoir plus de donnees
	 */
	if(dejaRecu < tailleMax) {
		retour = recv(socketClient, donnees + dejaRecu, tailleMax - dejaRecu, 0);
		if(retour < 0) {
			perror("ReceptionBinaire, erreur de recv.");
			return -1;
		} else if(retour == 0) {
			fprintf(stderr, "ReceptionBinaire, le serveur a ferme la connexion.\n");
			return 0;
		} else {
			/*
			 * on a recu "retour" octets en plus
			 */
			return dejaRecu + retour;
		}
	} else {
		return dejaRecu;
	}
}

/* Envoie des donn�es au serveur en pr�cisant leur taille.
 */
int EmissionBinaire(char *donnees, size_t taille) {
	int retour = 0;
	retour = send(socketClient, donnees, taille, 0);
	if(retour == -1) {
		perror("Emission, probleme lors du send.");
		return -1;
	} else {
		return retour;
	}
}

int pdu_request(struct requete_client *ptr_stru){
    size_t taille = snprintf(NULL, 0, "%c %s %s %c %s %s %s\n",ptr_stru -> type,ptr_stru -> login,ptr_stru -> password,ptr_stru -> ressource,ptr_stru -> target,ptr_stru -> attribute,ptr_stru -> payload) + 1; //On récupére la taille théorique de la string
    char  *chaine = malloc(taille); //On alloue un mémoire de la taille de la string
    fflush(stdin);
    sprintf(chaine, "%c %s %s %c %s %s %s\n",ptr_stru -> type,ptr_stru -> login,ptr_stru -> password,ptr_stru -> ressource,ptr_stru -> target,ptr_stru -> attribute,ptr_stru -> payload); // On écrit pour de bon dans la chaine
    
    if(Emission(chaine)!=1) {
        printf("Erreur d'émission\n");
        return 1;
    }
}

int pdu_reception(char *requete,int nbr_para,struct reponse_serveur *ptr_stru){ 
    char * str = (char *) malloc( (strlen(requete) + 1) * sizeof(char) );
    strcpy(str,requete); // Copie de la requete (strtok est destructeur)
	
	
	int i;
	
	int init_size = strlen(str);
	char delim[] = " ";

	char *ptr = strtok(str, delim);

	if(ptr != NULL){
		ptr_stru -> type = ptr[0];
		ptr = strtok(NULL, delim);
	}
	if(ptr != NULL){
		strcpy(ptr_stru -> login,ptr);
		ptr = strtok(NULL, delim);
	}
	if(ptr != NULL){
		ptr_stru -> code_retour = ptr[0];
		ptr = strtok(NULL, delim);
	}
	if(ptr != NULL){
		strcpy(ptr_stru -> payload,ptr);
		return 1; // success
	}
	
	return 0; // fail
    
}

void AffichageMenu(char *login)
{
	// Affichage du menu + choix de l'utilisateur
	system("clear");	// nettoyage de l'écran avec une fonction bash
	// Affichage du logo
	printf("\n.---.          _______         .-''-.     \n|   |          \\  ___ `'.    .' .-.  )    \n|   |           ' |--.\\  \\  / .'  / /     \n|   |           | |    \\  '(_/   / /      \n|   |    __     | |     |  '    / /       \n|   | .:--.'.   | |     |  |   / /        \n|   |/ |   \\ |  | |     ' .'  . '         \n|   |`\" __ | |  | |___.' /'  / /    _.-') \n|   | .'.''| | /_______.'/ .' '  _.'.-''  \n'---'/ /   | |_\\_______|/ /  /.-'_.'      \n     \\ \\._,\\ '/          /    _.'         \n      `--'  `\"          ( _.-'            \n\n\n");
	// Choix entre le menu de l'utilisateur ou le menu d'administrateur
	if(strcmp("admin", login) == 0)	
		printf("Menu d'administration\n\nChoisissez une action :\n\n   1. Obtenir une ressource\n   2. Créer un contact\n   3. Mettre à jour une information\n   4. Supprimer une information\n   5. Gérer les permissions\n   6. Quitter\n\n");
	else
		printf("Choisissez une action :\n\n   1. Obtenir une ressource\n   2. Créer un contact\n   3. Mettre à jour une information\n   4. Supprimer une information\n   5. Quitter\n\n");
}

int EnvoiRequeteGet(char *login, char *password)
{
	int choix;
	int retour = 0;
	int trame;
	requete_client req;

	// Construction de la requête client GET
	req.type = 0x00;
	strcpy(req.login, login);
	strcpy(req.password, password);
	strcpy(req.payload, "  ");

	system("clear");
	printf("Choisissez une ressource que vous voulez consulter :\n\n   1. Annuaire\n   2. Contact\n   3. Retour\n\n");

	while(retour != 1)
	{
		// Lecture du choix de l'utilisateur + vérification de saisie
		while(scanf("%d", &choix) != 1)
		{
			// vidage du buffer clavier
			while ((getchar()) != '\n');
			puts("Veuillez saisir un nombre entier :\n");
		}

		// Vidage du buffer clavier
		while ((getchar()) != '\n');

		switch(choix)
		{
			case 1:
				// Ajout des champs ressource et attribute à la trame
				req.ressource = 0x01;
				strcpy(req.attribute, "  ");

				printf("\nEntrez la première lettre du prénom et le nom de l'annuaire que vous souhaitez consulter (tout attaché)\nExemple : pour Martin Dupont, écrire 'mdupont'\n\n");
				// Lecture du choix du client avec vérifiacation de l'intégrité de la chaine
				if(fgets(req.target, 30, stdin) == NULL)
				{
					printf("Chaîne erronée.\n");
					return 1;
				}
				break;
			case 2:
				// Ajout des champs ressource et attribute à la trame
				req.ressource = 0x0A;
				strcpy(req.attribute, "  ");

				printf("\nEntrez la première lettre du prénom et le nom de du contact que souhaitez consulter (tout attaché)\nExemple : pour Martin Dupont, écrire 'mdupont'\n\n");
				// Lecture du choix du client avec vérifiacation de l'intégrité de la chaine
				if(fgets(req.target, 30, stdin) == NULL)
				{
					printf("Chaîne erronée.\n");
					return 1;
				}
				break;
			case 3:
				retour = 1;
				break;
			default:
				printf("Entrez un chiffre entre 1 et 3\n");
				break;
		}

		// On envoie la structure contenenant toutes les informations récoltées précedemmment pour construire la trame à envoyer 
		trame = pdu_request(&req);
		// On imprime le code de retour de la fonction
		printf("%d\n", trame);
	}
}

int EnvoiRequeteCreate(char *login, char *password)
{
	int choix;
	int trame;
	char payload[255], nom[30], prenom[30], email[30];
	requete_client req;

	// Construction de la requête client CREATE
	req.type = 0x01;
	req.ressource = 0x0A;
	strcpy(req.login, login);
	strcpy(req.password, password);
	strcpy(req.target, login);
	
	system("clear");

	// Vidage du buffer clavier
	while ((getchar()) != '\n');

	// Saisie du prénom du contact à créer + vérification
	puts("Entrez le prénom du contact à créer :\n");
	if(fgets(prenom, 30, stdin) == NULL)
	{
		printf("Chaîne erronée.\n");
		return 1;
	}

	// Saisie du nom du contact à créer + vérification
	puts("\nEntrez le nom du contact à créer :\n");
	if(fgets(nom, 30, stdin) == NULL)
	{
		printf("Chaîne erronée.\n");
		return 1;
	}

	// Saisie de l'email du contact à créer + vérification
	puts("\nEntrez l'Email du contact à créer :\n");
	if(fgets(email, 30, stdin) == NULL)
	{
		printf("Chaîne erronée.\n");
		return 1;
	}

	// Concaténation des données récupérées et insertion de la payload dans la trame
	sprintf(payload, "%s %s %s", prenom, nom, email);
	strcpy(req.payload, payload);

	// On envoie la structure contenenant toutes les informations récoltées précedemmment pour construire la trame à envoyer 
	trame = pdu_request(&req);
	// On imprime le code de retour de la fonction
	printf("%d\n", trame);

	sleep(2);

	return 0;
}

int EnvoiRequeteUpdate(char *login, char *password)
{
	int choix;
	int retour = 0;
	int trame;
	char payload[255], modif[30];
	requete_client req;

	// Construction de la requête client UPDATE
	req.type = 0x02;
	req.ressource = 0x0A;
	strcpy(req.login, login);
	strcpy(req.password, password);
	strcpy(req.target, login);
	
	system("clear");

	// Vidage du buffer clavier
	while ((getchar()) != '\n');

	puts("Entrez la première lettre du prénom et le nom de du contact que souhaitez modifier (tout attaché)\nExemple : pour Martin Dupont, écrire 'mdupont'\n");
	if(fgets(modif, 30, stdin) == NULL)
	{
		printf("Chaîne erronée.\n");
		return 1;
	}

	puts("\nChoisissez une ressource que vous voulez modifier :\n\n   1. Prénom\n   2. Nom\n   3. Email\n   4. Téléphone\n   5. Adresse Postale\n\n");

	while(retour != 1)
	{
		// Lecture du choix de l'utilisateur + vérification de saisie
		while(scanf("%d", &choix) != 1)
		{
			// vidage du buffer clavier
			while ((getchar()) != '\n');
			puts("Veuillez saisir un nombre entier :\n");
		}
		switch(choix)
		{
			case 1:
				strcpy(req.attribute, "prenom");
				retour = 1;
				break;

			case 2:
				strcpy(req.attribute, "nom");
				retour = 1;
				break;

			case 3:
				strcpy(req.attribute, "email");
				retour = 1;
				break;

			case 4:
				strcpy(req.attribute, "tel");
				retour = 1;
				break;

			case 5:
				strcpy(req.attribute, "adresse_postale");
				retour = 1;
				break;
			default:
				puts("Entrez un chiffre entre 1 et 5.");

				break;
		}
	}

	// vidage du buffer clavier
	while ((getchar()) != '\n');

	puts("Entrez la nouvelle valeur du champ à modifier :\n\n");
	if(fgets(req.payload, 100, stdin) == NULL)
	{
		printf("Chaîne erronée.\n");
		return 1;
	}

	// On envoie la structure contenenant toutes les informations récoltées précedemmment pour construire la trame à envoyer 
	trame = pdu_request(&req);
	// On imprime le code de retour de la fonction
	printf("%d\n", trame);

	sleep(2);

	return 0;
}

int EnvoiRequeteDelete(char *login, char *password)
{
	int choix;
	int retour = 0;
	int trame;
	char payload[255], suppr[30];
	requete_client req;

	// Construction de la requête client DELETE
	req.type = 0x02;
	req.ressource = 0x0A;
	strcpy(req.login, login);
	strcpy(req.password, password);
	strcpy(req.target, login);
	strcpy(req.payload, "  ");
	
	system("clear");

	// Vidage du buffer clavier
	while ((getchar()) != '\n');

	puts("Entrez la première lettre du prénom et le nom de du contact dont vous souhaitez supprimer du contenu (tout attaché)\nExemple : pour Martin Dupont, écrire 'mdupont'\n");
	if(fgets(suppr, 30, stdin) == NULL)
	{
		printf("Chaîne erronée.\n");
		return 1;
	}

	puts("\nChoisissez une ressource que vous voulez supprimer :\n\n   1. Prénom\n   2. Nom\n   3. Email\n   4. Téléphone\n   5. Adresse Postale\n\n");

	while(retour != 1)
	{
		// Lecture du choix de l'utilisateur + vérification de saisie
		while(scanf("%d", &choix) != 1)
		{
			// vidage du buffer clavier
			while ((getchar()) != '\n');
			puts("Veuillez saisir un nombre entier :\n");
		}
		switch(choix)
		{
			case 1:
				strcpy(req.attribute, "prenom");
				retour = 1;
				break;

			case 2:
				strcpy(req.attribute, "nom");
				retour = 1;
				break;

			case 3:
				strcpy(req.attribute, "email");
				retour = 1;
				break;

			case 4:
				strcpy(req.attribute, "tel");
				retour = 1;
				break;

			case 5:
				strcpy(req.attribute, "adresse_postale");
				retour = 1;
				break;
			default:
				puts("Entrez un chiffre entre 1 et 5.");

				break;
		}
	}

	// vidage du buffer clavier
	while ((getchar()) != '\n');

	// On envoie la structure contenenant toutes les informations récoltées précedemmment pour construire la trame à envoyer 
	trame = pdu_request(&req);
	// On imprime le code de retour de la fonction
	printf("%d\n", trame);

	sleep(2);

	return 0;
}


/* Ferme la connexion.
 */
void Terminaison() {
	close(socketClient);
}
