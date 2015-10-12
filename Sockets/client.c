#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>



int main(){
  int i, sock;
  struct sockaddr_in addr_send;
  char *server_ip ="192.168.0.106";
  unsigned short server_port = 63000;
  char buffer[1024];
  //Create the socket
  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0){
    printf("Failed to connect\n" );
  }
  printf("%d\n",sock );
  //Binding
  memset(&addr_send,0,sizeof(addr_send));
  addr_send.sin_family = AF_INET;
  addr_send.sin_addr.s_addr =  inet_addr(server_ip);
  addr_send.sin_port=htons(server_port);
  i=connect(sock, (struct sockaddr *) &addr_send, sizeof(addr_send));
  if ( i < 0){
    printf("Failed\n" );
  }

  strcpy(buffer, "Hello world");
  send(sock,buffer,strlen(buffer),0);
  return 0;

}
