#include "ClientFunctions.h"

int main(int argc, char *argv[]) {
  // argv[1] = 127.0.0.1
  // argv[2] = 8000

  launch(argv[1], atoi(argv[2]));
  return 0;
}
