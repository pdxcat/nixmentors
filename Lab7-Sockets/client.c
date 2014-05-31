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
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  int status, address_len, connection;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  int client_address_len;
  int accept_count = 0;
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

  // Attempt to bind on the desired address and port
  status = bind(sock, (struct sockaddr *) &server_address, sizeof(server_address));

  if(status == -1){
    fprintf(stderr, "Failed to bind socket, error no: %i\n", errno);
    exit(1);
  }

  // Start listening for new connections, allow up to 5 connections to queue without being accepted
  status = listen(sock, 5);

  if(status == -1){
    fprintf(stderr, "Failed to listen to address\n");
    exit(1);
  }

  accept_count = 0;
  // Accept five new connection before exiting
  while(accept_count < 5){
    client_address_len = sizeof(client_address);
    connection = accept(sock, (struct sockaddr *) &client_address, &client_address_len);
    if(connection == -1){
      fprintf(stderr, "Accept failed for some reason\n");
      exit(1);
    }
    printf("Got a new connection\n");

    // Fork a new process to handle the new connection
    connection_pid = fork();
    
    // In the child proccess
    if(connection_pid == 0){
      write(connection, message, strlen(message));
      status = shutdown(connection, SHUT_RDWR);
      if(status == -1){
        fprintf(stderr, "Failed to shutdown the connection\n");
      }
      close(connection);
    }
    
    // When forking fails
    if(connection_pid == -1){
        fprintf(stderr, "Failed to fork a new process to handle the new connection\n");
        close(connection);
        close(sock);
        exit(1);
    }

    accept_count++;
  }

  close(sock);

  printf("Goodbye!\n");
  return 0;
}
