#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "rwlock_rp.h"


#define M 5
#define N 5
#define ARRLEN 10240
int arr[ARRLEN];

int writers = 0;
int pers = 0;
pthread_mutex_t mutex;
pthread_cond_t condlect, condwrit;

void * escritor(void *arg) {
    int i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 3);
        write_lock(&mutex, &condlect, &pers, &writers);
        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++)
        arr[i] = num;
        write_unlock(&mutex, &condwrit, &writers);
    }
    return NULL;
}
void * lector(void *arg) {
    int v, i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 3);
        read_lock(&mutex, &condwrit, &pers, &writers);
        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
        if (arr[i] != v)
        break;
        }
        if (i < ARRLEN)
        printf("Lector %d, error de lectura\n", num);
        else
        printf("Lector %d, dato %d\n", num, v); 
        read_unlock(&mutex, &condlect, &pers);
    }
return NULL;
}
int main() {
pthread_t lectores[M], escritores[N];
pthread_mutex_init(&mutex, NULL);
pthread_cond_init(&condlect, NULL);
pthread_cond_init(&condwrit, NULL);
int i;
for (i = 0; i < N; i++)
pthread_create(&escritores[i], NULL, escritor, i + (void*)0);

for (i = 0; i < M; i++)
pthread_create(&lectores[i], NULL, lector, i + (void*)0);
pthread_join(lectores[0], NULL); /* Espera para siempre */
return 0;
}