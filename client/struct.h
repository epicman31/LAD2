typedef struct requete_client {
	char type;
	char ressource;
	char login[30];
	char password[30];
	char target[30];
	char attribute[30];
	char payload[255];
	}requete_client;

typedef struct reponse_serveur {
	char type;
	char login[30];
	char code_retour;
	char payload[1024];
	}reponse_serveur;