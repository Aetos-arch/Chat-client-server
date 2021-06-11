#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct struct_message {
  char *destinataire;
  char *message;
  int socket_client;
} struct_message;

typedef struct struct_envoyer_recevoir {
  int socket_client;
} struct_envoyer_recevoir;

void viderBuffer();

int lire(char *chaine, int longueur);

int envoyerMessage(int socket_client, char *message);

char *lireMessage(int sockId);

int analyseMessageRecuParServeur(char *messageServeur);

void envoiStructureMessage(struct_message *message);

void *lireReponses(void *reponse);

void *envoyerMessages(void *envoi);

int seConnecterAuServeur(char *ip, int port);

void lancer(char *ip, int port);
