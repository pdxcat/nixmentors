#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv){
  int sock = socket(AF_INET, SOCK_STREAM, 0);   // Request a file descriptor for the connection data
  int status, address_len, connection;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  int client_address_len;
  char* desired_address = "127.0.0.1";
  int desired_port = 8080;
  char* message = "Hello world\n";
  int bool_opt;
  socklen_t bool_opt_len = sizeof(bool_opt);
  pid_t connection_pid;

  if(sock == -1){
    fprintf(stderr, "Failed to open socket\n");
    exit(1);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port =  htons(desired_port);

  status = inet_aton(desired_address, &server_address.sin_addr);

  if(status == 0){
    fprintf(stderr, "Failed to parse address\n");
    exit(1);
  }

  connect( sock, server_address, bool_opt_len );

  close(sock);

  printf("Goodbye!\n");
  return 0;
}
