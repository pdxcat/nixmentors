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

  status = connect( sock, (struct sockaddr *) &server_address , bool_opt_len );

  if( status == 0 ){
    fprintf( stderr, "Failed to connect to remote: %s\n", desired_address );
    exit(1);
  }

  // write(connection, message, strlen(message));   // Save til read works.

  size_t buffer_length = 100;
  char buffer[ buffer_length + 1 ];
  size_t read_index = 0;
  ssize_t read_size = 0;

  while( ( read_size = read( sock, buffer + read_index, buffer_length - read_index ) ) != 0 ) {
    // The complex test on the above line calls read() for an amount of data not exceeding
    // the remaining amount of space in the buffer, until either the buffer is full, or the
    // remote stops sending.
    if( read_size == -1 ) {
      fprintf( stderr, "A read error occured\n" );
      exit(1);
    }

    read_index += read_size;
  }

  fprintf( stdout, "The remote server transmitted \"%s\"", buffer );

  close(sock);

  printf("Goodbye!\n");
  return 0;
}
