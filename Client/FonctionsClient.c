#include "FonctionsClient.h"

int arreterProgramme = 0;
int tube[2];

void viderBuffer() {
  int c = 0;
  while (c != '\n' && c != EOF) {
    c = getchar();
  }
}

int lire(char *chaine, int longueur) {
  char *positionEntree = NULL;

  if (fgets(chaine, longueur, stdin) != NULL) {
    positionEntree = strchr(chaine, '\n');
    if (positionEntree != NULL) {
      *positionEntree = '\0';
    } else {
      viderBuffer();
    }
    return 1;
  } else {
    viderBuffer();
    return 0;
  }
}

int envoyerMessage(int socket_client, char *message) {
  int envoie = send(socket_client, message, strlen(message) + 1, 0);
  if (envoie == -1) {
    perror("[Erreur d'envoi de message]");
    return -1;
  }
  return envoie;
}

char *lireMessage(int sockId) {
  char *message = (char *)malloc(sizeof(char));
  char c = 1;
  int i = 0;
  while (c != '\0') {
    recv(sockId, &c, 1, 0);
    *(message + i) = c;
    i++;
    message = realloc(message, (i + 1) * sizeof(char));
  }
  *(message + i) = '\0';
  return message;
}

int analyseMessageRecuParServeur(char *messageServeur) {
  if (strcmp(messageServeur, "-2") == 0) {
    perror("[Vous vous êtes connecté avec un nom qui existe déjà]");
  } else {
    return 0;
  }
}

void envoiStructureMessage(struct_message *message) {
  // -- Envoi du destinataire au serveur --
  envoyerMessage(message->socket_client, message->destinataire);
  // -- Envoi du message au serveur --
  envoyerMessage(message->socket_client, message->message);
}

void *lireReponses(void *reponse) {
  struct_envoyer_recevoir reponse_ = *((struct_envoyer_recevoir *)reponse);
  int socketClient = reponse_.socket_client;

  while (1) {
    char *destinataire = lireMessage(socketClient);
    char *expediteur = lireMessage(socketClient);
    char *message = lireMessage(socketClient);

    printf("\n---Contenu du message recu---\n\n");
    printf("  <- De : %s\n", expediteur);
    printf("  -> Pour : %s\n", destinataire);
    printf("  -> Message : %s\n", message);
    printf("\n------------------------\n\n");
  }
}

void *envoyerMessages(void *envoi) {
  struct_envoyer_recevoir envoi_ = *((struct_envoyer_recevoir *)envoi);
  int socketClient = envoi_.socket_client;

  char destinataire[20];
  printf("\nAvec qui voulez-vous communiquer ? (Maximum 20 caractères)\n");
  lire(destinataire, 20);
  char message[50];

  while (1) {
    printf(
        "\n-- Entrez votre message --\n(Maximum 50 caractères, 'changer' pour "
        "changer de destinataire, 'end' pour quitter le programme)\n");
    lire(message, 50);

    if (strcmp(message, "changer") == 0) {
      printf("\nAvec qui voulez-vous communiquer ? (Maximum 20 caractères)\n");
      lire(destinataire, 20);
      printf("\n-- Entrez votre message --\n(Maximum 50 caractères)\n");
      lire(message, 50);
    } else if (strcmp(message, "end") == 0) {
      arreterProgramme = 1;
      pthread_exit(0);
    }

    struct_message *messageAEnvoyer =
        (struct_message *)malloc(sizeof(struct_message));
    messageAEnvoyer->message = message;
    messageAEnvoyer->destinataire = destinataire;
    messageAEnvoyer->socket_client = socketClient;
    envoiStructureMessage(messageAEnvoyer);

    printf("\n  -> Message : [%s] bien envoyé\n", message);
  }
}

int seConnecterAuServeur(char *ip, int port) {
  struct sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port); // port du serveur
  sa.sin_addr.s_addr = (long)inet_addr(ip);

  // Creation Socket
  int sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd == -1) {
    perror("[Erreur de création de Socket]");
    return -1;
  }

  // Connexion
  int connexion = connect(sd, (struct sockaddr *)&sa, sizeof(sa));
  if (connexion == -1) {
    perror("[Erreur de connexion]");
    return -1;
  }
  printf("[Bien connecté au serveur]\n");
  return sd;
}

void lancer(char *ip, int port) {

  char nomClient[20];
  printf("Comment vous appelez-vous ? (Maximum 20 caractères)\n");
  lire(nomClient, 20);
  printf("Bonjour %s ! :D\n", nomClient);

  // Connexion serveur
  int socketClient = seConnecterAuServeur(ip, port);
  envoyerMessage(socketClient, nomClient);
  char *messageServeur = lireMessage(socketClient);
  analyseMessageRecuParServeur(messageServeur);
  
  pipe(tube);

  // Envoi
  pthread_t threadEnvoi;
  struct_envoyer_recevoir *envoi =
      (struct_envoyer_recevoir *)malloc(sizeof(struct_envoyer_recevoir));
  envoi->socket_client = socketClient;
  pthread_create(&threadEnvoi, NULL, (void *)envoyerMessages, (void *)envoi);

  // Reponse
  pthread_t threadReponse;
  struct_envoyer_recevoir *reponse =
      (struct_envoyer_recevoir *)malloc(sizeof(struct_envoyer_recevoir));
  reponse->socket_client = socketClient;
  pthread_create(&threadReponse, NULL, (void *)lireReponses, (void *)reponse);

  // Join
  pthread_join(threadEnvoi, NULL);
  pthread_cancel(threadReponse);

  // Fin
  envoyerMessage(socketClient, "end");
  close(socketClient);
  printf("[Bien déconnecté, au revoir %s !]\n", nomClient);
}