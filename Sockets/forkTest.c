#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>

int main(){
  static int *random;
  int parentID;
  int bufferArray[10];
  static int *currentIndex;
  srand(time(NULL));
  random = mmap(NULL, sizeof *random, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  currentIndex = mmap(NULL, sizeof *currentIndex, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  *currentIndex = 0;

  parentID = getpid();
  int producers[2];
  int numberOfProducers;
  for (numberOfProducers = 0; numberOfProducers < 2; numberOfProducers++){
    producers[numberOfProducers] = fork();
    if (producers[numberOfProducers]) continue;
    else if(producers[numberOfProducers] == 0) break;
    else{
      printf("Fork error\n" );
    }
  }

  while(1){
    if(parentID == getpid()){
      *random = rand() % 3;
    }else{
      if ((getpid()+*random) == getpid()){
        //produce
        if (*currentIndex == 10){
          printf("The buffer is full, sleeping process %d \n", *random );
          sleep(2);
        }else{
          printf("Producer %d is producing an item\n", *random);
          bufferArray[*currentIndex] = 1;
          *currentIndex = *currentIndex + 1;
        }
      }else{
        //printf("Sleeping producer %d\n", *random);
        sleep(2);
      }
    }
  }

  return 1;
}
