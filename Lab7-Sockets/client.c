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
  // Create a temp to catch return codes from various functions.
  int status;

  // Define various default values if they're not passed in on the command line.
  char* desired_address = malloc(16);
  int desired_port = 8080;
  char* message = "Hello world\n";
  // Set the remote address to use.
  if( argc > 1 )
    strncpy(desired_address, argv[1], 15 );
  else {
    fprintf( stdout, "No remote address specified, defaulting to localhost\n" );
    strcpy(desired_address, "127.0.0.1");
  }
  // Set the remote port to use, if specified.
  if( argc > 2 )
    desired_port = atoi(argv[2]);
  else
    fprintf( stdout, "No remote port specified, defaulting to 8080\n" );

  // Request a file descriptor from the OS upon which we will build our connection.
  int sock = socket(AF_INET, SOCK_STREAM, 0);   
  // Error check for a failed socket allocation.
  if(sock == -1){
    fprintf(stderr, "Failed to open socket\n");
    fprintf( stderr, "ERRNO: %d, Error: %s\n", errno, strerror(errno) );
    exit(1);
  }

  // Prepare a struct that defines the remote server to connect to.
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port =  htons(desired_port);
  // Translate the human-readable remote address to its binary equivalent.
  status = inet_aton(desired_address, &server_address.sin_addr);
  // Make sure the IP address was translated correctly - a failure at this stage may mean we got
  // a bad address.
  if(status == -1){
    fprintf(stderr, "Failed to parse address: %s\nERRNO: %d, Error: %s\n", desired_address, errno, strerror(errno) );
    exit(1);
  }

  // int bool_opt;
  // socklen_t bool_opt_len = sizeof(bool_opt);
  socklen_t bool_opt_len = 32;
  // sizeof() returns a length in bytes. connect() expects a number that represents an int, but
  // it expects it in bits. Hardcoding the above value may cause unexpected behavior with other
  // socket types, but this shouldn't be relevant for ipv4 connections (32 bits by definition).
  //
  // Call connect() to negotiate a TCP handshake with the remote host, and capture its return code.
  status = connect( sock, (struct sockaddr *) &server_address, bool_opt_len );
  // Check the return code. On fail, report the error and exit.
  if( status == -1 ){
    fprintf( stderr, "Failed to connect to remote: %s\n", desired_address );
    fprintf( stderr, "ERRNO: %d, Error: %s\n", errno, strerror(errno) );
    exit(1);
  }
  else {
    fprintf( stdout, "Successfully connected to remote: %s\n", inet_ntoa(server_address.sin_addr) );
  }

  // write(connection, message, strlen(message));   // Save this til read works.

  // Prepare some values we'll need to do a robust sequential read from the remote.
  size_t buffer_length = 100;
  char buffer[ buffer_length + 1 ];
  size_t read_index = 0;
  ssize_t read_size = 0;
  //
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

  // Display what we read from the remote host.
  fprintf( stdout, "The remote server transmitted \"%s\"", buffer );

  // Clean up and go home.
  close(sock);
  printf("Goodbye!\n");
  return 0;
}
