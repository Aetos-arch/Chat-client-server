#include "ClientFunctions.h"

int stopProgram = 0;

void clearBuffer() {
  int c = 0;
  while (c != '\n' && c != EOF) {
    c = getchar();
  }
}

int readInput(char *string, int length) {
  char *newlinePos = NULL;

  if (fgets(string, length, stdin) != NULL) {
    newlinePos = strchr(string, '\n');
    if (newlinePos != NULL) {
      *newlinePos = '\0';
    } else {
      clearBuffer();
    }
    return 1;
  } else {
    clearBuffer();
    return 0;
  }
}

int sendMessage(int clientSocket, char *message) {
  int sent = send(clientSocket, message, strlen(message) + 1, 0);
  if (sent == -1) {
    perror("[Error sending message]");
    return -1;
  }
  return sent;
}

char *readMessage(int socketId) {
  char *message = (char *)malloc(sizeof(char));
  char c = 1;
  int i = 0;
  while (c != '\0') {
    recv(socketId, &c, 1, 0);
    *(message + i) = c;
    i++;
    message = realloc(message, (i + 1) * sizeof(char));
  }
  *(message + i) = '\0';
  return message;
}

int analyzeServerMessage(char *serverMessage) {
  if (strcmp(serverMessage, "-2") == 0) {
    perror("[You have connected with an existing name]");
  } else {
    return 0;
  }
}

void sendStructureMessage(struct_message *message) {
  // -- Send recipient to the server --
  sendMessage(message->clientSocket, message->recipient);
  // -- Send message to the server --
  sendMessage(message->clientSocket, message->message);
}

void *readResponses(void *response) {
  struct_send_receive response_ = *((struct_send_receive *)response);
  int clientSocket = response_.clientSocket;

  while (1) {
    char *recipient = readMessage(clientSocket);
    char *sender = readMessage(clientSocket);
    char *message = readMessage(clientSocket);

    printf("\n---Received Message Content---\n");
    printf("  From: %s\n", sender);
    printf("  To: %s\n", recipient);
    printf("  Message: %s\n", message);
    printf("-----------------------------\n");
  }
}

void *sendMessages(void *send) {
  struct_send_receive send_ = *((struct_send_receive *)send);
  int clientSocket = send_.clientSocket;

  char recipient[20];
  printf("\nWho do you want to communicate with? (Maximum 20 characters)\n");
  readInput(recipient, 20);
  char message[50];

  while (1) {
    printf("\n-- Enter your message --\n");
    printf("(Maximum 50 characters, 'change' to change recipient, 'end' to quit the program)\n");
    readInput(message, 50);

    if (strcmp(message, "change") == 0) {
      printf("\nWho do you want to communicate with? (Maximum 20 characters)\n");
      readInput(recipient, 20);
      printf("\n-- Enter your message --\n(Maximum 50 characters)\n");
      readInput(message, 50);
    } else if (strcmp(message, "end") == 0) {
      stopProgram = 1;
      pthread_exit(0);
    }

    struct_message *messageToSend = (struct_message *)malloc(sizeof(struct_message));
    messageToSend->message = message;
    messageToSend->recipient = recipient;
    messageToSend->clientSocket = clientSocket;
    sendStructureMessage(messageToSend);

    printf("\nMessage [%s] sent successfully.\n", message);
  }
}

int connectToServer(char *ip, int port) {
  struct sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port); // server port
  sa.sin_addr.s_addr = (long)inet_addr(ip);

  // Create Socket
  int sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd == -1) {
    perror("[Socket creation error]");
    return -1;
  }

  // Connect
  int connection = connect(sd, (struct sockaddr *)&sa, sizeof(sa));
  if (connection == -1) {
    perror("[Connection error]");
    return -1;
  }
  printf("[Connected to the server]\n");
  return sd;
}

void launch(char *ip, int port) {

  char clientName[20];
  printf("What is your name? (Maximum 20 characters)\n");
  readInput(clientName, 20);
  printf("Hello %s! :D\n", clientName);

  // Connect to server
  int clientSocket = connectToServer(ip, port);
  sendMessage(clientSocket, clientName);
  char *serverMessage = readMessage(clientSocket);
  analyzeServerMessage(serverMessage);

  // Sending
  pthread_t sendThread;
  struct_send_receive *sendData = (struct_send_receive *)malloc(sizeof(struct_send_receive));
  sendData->clientSocket = clientSocket;
  pthread_create(&sendThread, NULL, (void *)sendMessages, (void *)sendData);

  // Receiving
  pthread_t receiveThread;
  struct_send_receive *receiveData = (struct_send_receive *)malloc(sizeof(struct_send_receive));
  receiveData->clientSocket = clientSocket;
  pthread_create(&receiveThread, NULL, (void *)readResponses, (void *)receiveData);

  // Join
  pthread_join(sendThread, NULL);
  pthread_cancel(receiveThread);

  // End
  sendMessage(clientSocket, "end");
  close(clientSocket);
  printf("[Disconnected. Goodbye, %s!]\n", clientName);
}
