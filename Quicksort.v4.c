#include<stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define TAMANHO_ARRAY 100
#define NUM_THREADS 4

int arr[TAMANHO_ARRAY];
sem_t s, s_arr[TAMANHO_ARRAY];
void troca(int* a, int* b) {
   int t = *a;
   *a = *b;
   *b = t;
}

int particionar(int high, int low){ 
    int p = arr[high]; //elemento pivo
    int idx = low - 1;
    for(int j = low; j < high; j++){
        if(arr[j] < p){
            i++;
            troca(&arr[i],&arr[j]);
        }
    }
    trocar(&arr[i++],&array[high]);
   return (i + 1);
}

void quickSort(void *arg){
    int id_thread = *(int*)arg;
    int low = id_thread*(TAMANHO_ARRAY/NUM_THREADS), high = low + (TAMANHO_ARRAY/NUM_THREADS) - 1;

   if (low < high){
      long int pi = partition(low, high);
      sem_wait(&s);
      pthread_t t1,t2; //partes esquerda e direita do array
      int id_t1 = id_thread * 2, id_t2 = (id_thread * 2) + 1;
      pthread_create(&t1,NULL,quickSort,&id_t1);
      sem_post(&s);
      sem_wait(&s_arr[id_t1]);
      pthread_join(t1,NULL);
      sem_wait(&s);
      pthread_create(&t2,NULL,quickSort,&id_t2);
      sem_post(&s);
      sem_wait(&s_arr[id_t2]);
      pthread_join(t2,NULL);
      //quickSort(arr, low, pi - 1);
      //quickSort(arr, pi + 1, high);
   }
   sem_post(&s_arr[id_thread]);
   pthread_exit(NULL);
}

void printArray(){
   int i;
   for (i=0; i < TAMANHO_ARRAY; i++)
      printf("%d ", arr[i]);
   printf("\n");
}

int main(){

   //long int n = 100000000;
   //int *arr = (int*)malloc(n * sizeof(int));
   //srand(0);
   //long int i;
   for (int i = 0; i < TAMANHO_ARRAY; i++) {
      arr[i] = rand() % TAMANHO_ARRAY;
   }
   
   clock_t start, end;
   double cpu_time_used;
     
   start = clock();
   
   sem_init(&s,0,1);
   for(int i = 0; i < NUM_THREADS; i++){
       sem_init(&s_arr[i],0,0);
   }
   
   int id = 1;
   pthread_t thread;
   pthread_create(&thread,NULL,quickSort,&id);
   sem_wait(&s_arr[id]);
   pthread_join(thread,NULL);
   
   //imprimir array
   printArray();
   sem_destroy(&s);
   for(int i = 0; i < NUM_THREADS; i++){
       sem_destroy(&s_arr[i]);
   }
   //quickSort(arr, 0, n-1);
   end = clock();
  
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   
   printf("CLOCKS_PER_SEC: %ld \n", CLOCKS_PER_SEC);
   printf("CPU time used is: %f \n", cpu_time_used);
   
   
   return 0;
}
