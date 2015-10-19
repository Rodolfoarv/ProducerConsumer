#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include "globalVariables.h"

int main(){
  int i,n, sock, sock_recv, clientLen;
  struct sockaddr_in my_addr;
  struct sockaddr_in recv_addr;
  unsigned short listen_port = 61030;
  char buffer[256];

  //Producer variables
  static int *random;
  int parentID;
  int bufferArray[10];
  static int *currentIndex;

  random = mmap(NULL, sizeof *random, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  srand(time(NULL));
  *random = rand() % 3;
  currentIndex = mmap(NULL, sizeof *currentIndex, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *currentIndex = 0;
  parentID = getpid();

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

    if (sock_recv < 0){
      perror("ERROR on accept");
      exit(1);
    }
    //This means that the connection has been established and we're able to communicate

    //Create the children
    int producers[10];
    int numberOfProducers;
    for (numberOfProducers = 0; numberOfProducers < 10; numberOfProducers++){
      producers[numberOfProducers] = fork();
      if (producers[numberOfProducers]) continue;
      else if(producers[numberOfProducers] == 0) break;
      else{
        printf("Fork error\n" );
      }
    }

    while(1){
      *random = rand() % 10;
      if(parentID == getpid()){

        //the parent will listen to requests
        bzero(buffer,256);
        n = read(sock_recv,buffer,255);
        if (n < 0){
          perror("ERROR reading from socket");
          exit(1);
        }
        printf("Here is the message: %s\n",buffer);
        bufferArray[*currentIndex] = 0;
        *currentIndex = *currentIndex -1;



      }else{

        if ((parentID+*random) == getpid()){
          //produce
          if (*currentIndex == 10){
            printf("The buffer is full, sleeping process %d \n", getpid() );
            //n = write(sock_recv,"produce",7);
            sleep(2);
          }else{
            printf("************** Producer %d is producing an item **************** \n", *random);
            bufferArray[*currentIndex] = 1;
            *currentIndex = *currentIndex + 1;
            n = write(sock_recv,"produce",7);
            sleep(6); //give the opportunity for another process to get into the production section
          }
        }else{
          //printf("Sleeping producer with ID %d\n", getpid());
          sleep(1);
        }
      }
    }

  return 0;
}
