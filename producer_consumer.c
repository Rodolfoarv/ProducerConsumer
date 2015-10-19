#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>


static int *currentIndex; // variable shared between all forks
static int *randomProducer; //variable that will distribute the work among the producers
static int *randomConsumer; //variable that will distribute the work among the consumers
int parentID; // variable that will be summed to get the current child
void producers(); //method that will create and handle the producers
void consumers(); //method that will create and handle the consumers

int main(){
  currentIndex = mmap(NULL, sizeof *currentIndex, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *currentIndex = 0;

  randomProducer = mmap(NULL, sizeof *randomProducer, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  srand(time(NULL));
  *randomProducer = rand() % 4;

  randomConsumer = mmap(NULL, sizeof *randomConsumer, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  srand(time(NULL));
  *randomConsumer = rand() % 4;

  parentID = getpid();
  producers();
  consumers();
  while (1){
    //Parent that will do nothing
  }
  return 1;
}

//Routine that will create and handle the producers
void producers(){
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
  if (getpid() != parentID){
    while(1){ //Infinite loop that will produce all the time unless the buffer is full
      if (*currentIndex == 7){
        //The buffer is full
        sleep(3);
      }else{
        *randomProducer = rand() % 4;
        if ((parentID + *randomProducer) == getpid()){
          printf("Producer %d is producing an item\n", *randomProducer );
          *currentIndex = *currentIndex + 1;
          sleep(5);
        }else{
          sleep(3);
        }
      }
    }
  }
}

//Routine that will create and handle the consumers
void consumers(){
  int consumers[10]; //number of consumers
  int numberOfConsumers;
  for (numberOfConsumers = 0; numberOfConsumers < 10; numberOfConsumers++){
    consumers[numberOfConsumers] = fork();
    if (consumers[numberOfConsumers]) continue;
    else if(consumers[numberOfConsumers] == 0) break;
    else{
      printf("Fork error\n" );
    }
  }

  if (getpid() != parentID){
    while(1){ //Infinite loop that will consume all the time unless the buffer is empty
      if (*currentIndex == 0){
        //Buffer is empty
        sleep(3);
      }else{
        *randomConsumer = (rand() % 10)+10;
        if ((parentID + *randomConsumer) == getpid()){
          printf("Consumer %d is consuming an item\n", *randomConsumer-10 );
          *currentIndex = *currentIndex - 1;
          sleep(5);
        }else{
          sleep(3);
        }
      }

    }
  }
}
