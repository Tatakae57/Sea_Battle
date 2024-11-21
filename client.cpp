#include "client.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

// Constants
#define PORT 8000

// Variables
//  Locals
static char buffer[1024];
static int client;
static struct sockaddr_in server_address;

//  Externals
extern char ip[20], confirm[2];
extern bool confirmations[2];

// Functions
void wait_message() {
  recv(client, buffer, sizeof(buffer), 0);
}

void create_socket() {
  // Create socket
  if ((client = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    confirm[0] = 2; // Show failed socket creation
  }
  else {
    confirm[0] = 1; // Show socket created
    confirmations[1] = true; // Show trying connection
    // Setting server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(ip);
    connect_to_server();
  }
}

void connect_to_server() {
  if (connect(client, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    confirm[1] = 2; // Show failed connection.
  }
  else confirm[1] = 1; // Show connection succesfull
}
