#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "client.h"

struct requete_client {
	char type;
	char ressource;
	char login[30];
	char password[30];
	char target[30];
	char attribute[30];
	char payload[250];
	};