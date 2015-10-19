#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>



int main(){
  int i, n,sock;
  struct sockaddr_in addr_send;
  char *server_ip ="10.48.126.237";
  unsigned short server_port = 64000;
  char buffer[256];

  //Consumer variables
  int parentID;
  static int *currentIndex;
  static int *random;
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
  memset(&addr_send,0,sizeof(addr_send));
  addr_send.sin_family = AF_INET;
  addr_send.sin_addr.s_addr =  inet_addr(server_ip);
  addr_send.sin_port=htons(server_port);
  i=connect(sock, (struct sockaddr *) &addr_send, sizeof(addr_send));
  if ( i < 0){
    printf("Failed\n" );
  }
  //Create the consumers

  int consumers[10];
  int numberOfConsumers;
  for (numberOfConsumers = 0; numberOfConsumers < 10; numberOfConsumers++){
    consumers[numberOfConsumers] = fork();
    if (consumers[numberOfConsumers]) continue;
    else if(consumers[numberOfConsumers] == 0) break;
    else{
      printf("Fork error\n" );
    }
  }
  sleep(10);
  while (1){
    *random = rand() % 10;
    if (parentID == getpid()){
      n = read(sock,buffer,255);
      if (n < 0){
        perror("ERROR reading from socket");
        exit(1);
      }
      *currentIndex = *currentIndex +1;
      printf("An item has been produced, we're ready to consume\n" );
    }else{
      if ((parentID +*random) == getpid()){

        if (*currentIndex == 0){
          printf("The buffer isn't ready yet\n");
          sleep(5);
        }else{
          printf("************** Consumer %d is consuming an item **************** \n", *random);
          /* Send message to the server */
          n = write(sock, "consume", 7);
          *currentIndex = *currentIndex - 1;
          if (n < 0)
          {
             perror("ERROR writing to socket");
             exit(1);
          }

          sleep(6);
        }
      }else{
        sleep(3);
      }
    }
  }


  return 0;

}
