#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "serveur.h"


int main() {
	char *message = NULL;
	struct requete_client rq;
	int nbr_para = 6;


	char code_retour = 'f';
	char type = 'c';
	char payload[1024];
	char login[30];
	Initialisation();

	while(1) {
		int fini = 0;
	
		AttenteClient();
	
		while(!fini) {
			message = Reception();

			if(message != NULL) {
				//printf("J'ai recu: %s\n", message);
				pdu_reception(message,nbr_para,&rq);
				printf("%s\n",rq.login);
				printf("%s\n",rq.password);
				strcpy(login,rq.login);
				strcpy(payload,rq.payload);
				free(message);

				if(pdu_response(type,login,code_retour,payload)!=1) {
					printf("Erreur d'emission\n");
				}
			} else {
				fini = 1;
			}
		}

		TerminaisonClient();
	}

	return 0;
}
