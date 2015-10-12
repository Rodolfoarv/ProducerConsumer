#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
static int *currentIndex;
int *buffer;
int size;


/*
typedef struct buffer{
  int size;
  int array[30];
} Buffer;
*/

int main(){
  size = 30;
  currentIndex = mmap(NULL, sizeof *currentIndex, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *currentIndex = 0;
  buffer = mmap(NULL, size*sizeof(int),PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,-1, 0);

  //void itemIsReady(int);
  //signal(SIGUSR1, itemIsReady);

  int i = fork();
  //signal(SIGUSR2, p);

  if (i != 0){ //Productor
    while (1){
      if (*currentIndex < size){
        buffer[*currentIndex] = 1;
        *currentIndex+= 1;
        printf("Produced an item in position %d\n",*currentIndex );
      }else{
        sleep(1);
      }
    }

  }else{ //Consumidor
    while (1){
      if (*currentIndex == 0){
        sleep(1);
      }else{
        buffer[*currentIndex] = 0;
        *currentIndex-= 1;
        printf("Consumed an item in position %d\n",*currentIndex );
      }
    }

  }



  return 1;
}

void itemIsReady(int signum){
  printf("We got an item\n" );

}
