#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(){
  int i, sock, sock_recv;
  struct sockaddr_in my_addr;
  struct sockaddr_in recv_addr;
  unsigned short listen_port = 62000;
  char text[80];

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
  if (i < 0){
    printf("Listen has failed\n" );
  }
  socklen_t client_len;
  sock_recv=accept(sock, (struct sockaddr *) &recv_addr, &client_len);
  recv(sock_recv, text, 80,0);
  printf("Data recieved %s \n",text );
  i = close(sock_recv);




  return 0;
}
