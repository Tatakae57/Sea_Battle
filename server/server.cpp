#include <cstdlib>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <string.h>

// Constants
#define PLAYERS 2
#define PORT 8000
#define POSITIONS 10
const char *ip = "127.0.0.1";

// Variables
//  net
unsigned char connected_clients = 0;
int clients[PLAYERS];
char buffers[PLAYERS][2048];

//  Game
unsigned char map[PLAYERS][POSITIONS][POSITIONS];

// Functions
void start_server(int &server) {
  // Creating socket
  server = socket(AF_INET, SOCK_STREAM, 0); // Setting TCP socket

  printf("Creating socket... ");
  if (server < 0) {
    printf("failed.\n");
    exit(EXIT_FAILURE);
  }
  printf("done.\n");

  // Setting server address
  sockaddr_in server_addr;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  // Binding server
  printf("Starting server... ");
  if (bind(server, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    printf("failed.\n");
    exit(EXIT_FAILURE);
  }
  printf("done.\n");
  listen(server, 2);
  printf("Server started at: %s:%d\n", ip, PORT);
}

void wait_for_clients(int &server) {
  while (connected_clients != PLAYERS) {
    clients[connected_clients] = accept(server, nullptr, nullptr);
    printf("Player %d connected.\n", connected_clients + 1);
    connected_clients++;
  }
}

void start_game(int &server) {
  sleep(3); // Wait 3 seconds
  for (unsigned char i = 0; i != PLAYERS; i++) {
    printf("Sending start signal to Player %d.\n", i + 1);
    send(clients[i], "start", sizeof(char) * 5, 0); // Send the start signal for the players
  }
}

void wait_clients_messages(int client, char *buffer) {
  recv(client, buffer, sizeof(buffer), 0);
  if (!strcmp(buffer, "ready")) {
    printf("%d is ready.\n", client);
  }
}

// Main
int main() {
  // Start server
  int server;
  start_server(server);

  // Wait for clients (2)
  wait_for_clients(server);
    
  // Setting clients threads
  std::thread th1(wait_clients_messages, clients[0], buffers[0]);
  std::thread th2(wait_clients_messages, clients[1], buffers[1]);
  // Starting threads
  th1.detach();
  th2.detach();

  // Sending start signal
  start_game(server);

  // Wait for players ready
  while (strcmp(buffers[0], "ready") || strcmp(buffers[1], "ready")) {
    sleep(1);
  }
  
  close(server); // Close server
  return 0;
}
