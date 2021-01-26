#ifndef __CLIENT_H__
#define __CLIENT_H__



/* Initialisation.
 * Connexion au serveur sur la machine donnee.
 * Utilisez localhost pour un fonctionnement local.
 * renvoie 1 si �a c'est bien pass� 0 sinon
 */
int Initialisation(char *machine);

/* Initialisation.
 * Connexion au serveur sur la machine donnee et au service donne.
 * Utilisez localhost pour un fonctionnement local.
 * renvoie 1 si �a c'est bien pass� 0 sinon
 */
int InitialisationAvecService(char *machine, char *service);

/* Recoit un message envoye par le serveur.
 * retourne le message ou NULL en cas d'erreur.
 * Note : il faut liberer la memoire apres traitement.
 */
char *Reception();

/* Envoie un message au serveur.
 * Attention, le message doit etre termine par \n
 * renvoie 1 si �a c'est bien pass� 0 sinon
 */
int Emission(char *message);

/* Recoit des donnees envoyees par le serveur.
 * renvoie le nombre d'octets re�us, 0 si la connexion est ferm�e,
 * un nombre n�gatif en cas d'erreur
 */
int ReceptionBinaire(char *donnees, size_t tailleMax);

/* Envoie des donn�es au serveur en pr�cisant leur taille.
 * renvoie le nombre d'octets envoy�s, 0 si la connexion est ferm�e,
 * un nombre n�gatif en cas d'erreur
 */
int EmissionBinaire(char *donnees, size_t taille);

/* Fonction qui envoie la requéte au serveur avec la structure du protocole
*/
int pdu_request(struct requete_client *ptr_stru);

/* Fonction qui adapte la requete dans une structure
*/
int pdu_reception(char *requete,int nbr_para,struct reponse_serveur *ptr_stru);

/* Permet l'affichage du menu principal du programme */
void AffichageMenu(char *login);

/* Fonction qui permet d'envoyer au serveur une requete de type get */ 
int EnvoiRequeteGet(char *login, char *password);

/* Fonction qui permet d'envoyer au serveur une requete de type create */ 
int EnvoiRequeteCreate(char *login, char *password);

/* Fonction qui permet d'envoyer au serveur une requete de type update */ 
int EnvoiRequeteUpdate(char *login, char *password);

/* Fonction qui permet d'envoyer au serveur une requete de type delete */
int EnvoiRequeteDelete(char *login, char *password);

/* Ferme la connexion.
 */
void Terminaison();

#endif
