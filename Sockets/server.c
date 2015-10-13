#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(){
  int i,n, sock, sock_recv, clientLen;
  struct sockaddr_in my_addr;
  struct sockaddr_in recv_addr;
  unsigned short listen_port = 60000;
  char buffer[256];

  //Create the socket
  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0){
    printf("Failed to connect\n" );
  }
  printf("%d\n",sock );
  //Binding
  memset(&my_addr,0,sizeof(my_addr)); // clear structure
  my_addr.sin_family = AF_INET; // address family
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY); //my ip
  my_addr.sin_port = htons(listen_port);
  i=bind(sock, (struct sockaddr*) &my_addr,sizeof(my_addr));
  if (i < 0){
    printf("Failed miserably\n" );
  }

  i = listen(sock,5);
  socklen_t client_len;
  sock_recv=accept(sock, (struct sockaddr *) &recv_addr, &client_len);
  while (1){
    if (sock_recv < 0){
      perror("ERROR on accept");
      exit(1);
    }
    //This means that the connection has been established and we're able to communicate
    bzero(buffer,256);
    n = read(sock_recv,buffer,255);
    if (n < 0){
      perror("ERROR reading from socket");
      exit(1);
    }
    printf("Here is the message: %s\n",buffer);
    n = write(sock_recv,"I got your message",18);
    if (n < 0){
       perror("ERROR writing to socket");
       exit(1);
    }

  }

  return 0;
}
