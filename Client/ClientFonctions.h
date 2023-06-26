#include <arpa/inet.h>
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
  char *recipient;
  char *message;
  int clientSocket;
} struct_message;

typedef struct struct_send_receive {
  int clientSocket;
} struct_send_receive;

void clearBuffer();

int readInput(char *chaine, int longueur);

int sendMessage(int clientSocket, char *message);

char *readMessage(int sockId);

int analyzeServerMessage(char *serverMessage);

void sendStructureMessage(struct_message *message);

void *readResponses(void *response);

void *sendMessages(void *sendData);

int connectToServer(char *ip, int port);

void launch(char *ip, int port);
