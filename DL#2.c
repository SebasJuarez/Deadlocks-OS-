#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t forks[NUM_PHILOSOPHERS];
sem_t dining_limit;

void *philosopher(void *arg) {
    int id = *((int *)arg);
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    while (1) {
        // Pensar
        printf("Filosofo %d esta pensando\n", id);

        // Esperar a que el límite de comensales permitidos sea menor a 4
        sem_wait(&dining_limit);

        // Tomar los tenedores
        printf("Filosofo %d esta intentando tomar el tenedor izquierdo %d\n", id, left_fork);
        pthread_mutex_lock(&forks[left_fork]);
        printf("Filosofo %d tomo el tenedor izquierdo %d\n", id, left_fork);
        
        printf("Filosofo %d esta intentando tomar el tenedor derecho %d\n", id, right_fork);
        pthread_mutex_lock(&forks[right_fork]);
        printf("Filosofo %d tomo el tenedor derecho %d\n", id, right_fork);

        // Comer
        printf("Filosofo %d esta comiendo\n", id);
        sleep(2);

        printf("Filosofo %d esta soltando el tenedor derecho %d\n", id, right_fork);
        pthread_mutex_unlock(&forks[right_fork]);
        
        printf("Filosofo %d esta soltando el tenedor izquierdo %d\n", id, left_fork);
        pthread_mutex_unlock(&forks[left_fork]);

        sem_post(&dining_limit);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    sem_init(&dining_limit, 0, NUM_PHILOSOPHERS - 1);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    sem_destroy(&dining_limit);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
