/****************** CLIENT CODE ****************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>


int main(int argc, char **argv){
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  if (argc != 3){
    fprintf(stderr, "Usage: %s hostname portNumber\n", argv[0]);
    exit(1);
  }

  struct hostent *server;
  server = gethostbyname(argv[1]); //first argument is host name
  printf("host ip: %s\n", inet_ntoa(*(struct in_addr*)server->h_addr_list[0]));

  int portNumber = atoi(argv[2]); //second argument is the port number

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(portNumber);

  /* Set IP address for server */
  //1. old code to set server address to localhost
  //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  //2. new code to set server address to the address of the hostname
  //provided as an argument 
  //(e.g., when running the command (./client www.google.com))
  //but when running the program on a local muchine you'll want 
  //to run (./client localhost) in order to communicate with the server
  bcopy((char *)server->h_addr, //do man bcopy on the terminal
       (char *)&serverAddr.sin_addr.s_addr,
        server->h_length);
  
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  /*---- Read the message from the server into the buffer ----*/
  recv(clientSocket, buffer, 1024, 0);

  /*---- Print the received message ----*/
  printf("Data received: %s",buffer);   

  return 0;
}