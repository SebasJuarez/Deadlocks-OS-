#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_RESOURCES 4
#define NUM_PROCESSES 3

int available[NUM_RESOURCES] = {3, 3, 2, 2}; // Recursos disponibles
int maximum[NUM_PROCESSES][NUM_RESOURCES] = {{7, 5, 3, 2}, {3, 2, 2, 2}, {9, 0, 2, 2}}; // Máximos requeridos por proceso
int allocation[NUM_PROCESSES][NUM_RESOURCES] = {{0, 1, 0, 0}, {2, 0, 0, 1}, {3, 0, 2, 0}}; // Asignados actualmente a cada proceso
int need[NUM_PROCESSES][NUM_RESOURCES]; // Necesidades restantes de cada proceso

bool finish[NUM_PROCESSES] = {false, false, false}; // Indica si el proceso ha terminado

// Inicializa las necesidades restantes de cada proceso
void initialize() {
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

// Verifica si un proceso puede solicitar recursos de manera segura
bool isSafe(int process) {
    int work[NUM_RESOURCES];
    for (int i = 0; i < NUM_RESOURCES; i++) {
        work[i] = available[i];
    }

    bool finishCopy[NUM_PROCESSES];
    for (int i = 0; i < NUM_PROCESSES; i++) {
        finishCopy[i] = finish[i];
    }

    // Intenta asignar recursos y verifica si el estado es seguro
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (need[process][i] > work[i]) {
            return false;
        }
    }

    for (int i = 0; i < NUM_RESOURCES; i++) {
        work[i] -= need[process][i];
        allocation[process][i] += need[process][i];
        need[process][i] = 0;
    }

    // Verifica si el estado es seguro después de la asignación
    bool flag;
    do {
        flag = false;
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (!finishCopy[i]) {
                int j;
                for (j = 0; j < NUM_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == NUM_RESOURCES) {
                    for (int k = 0; k < NUM_RESOURCES; k++) {
                        work[k] += allocation[i][k];
                    }
                    finishCopy[i] = true;
                    flag = true;
                }
            }
        }
    } while (flag);

    // Verifica si todos los procesos se han completado
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (!finishCopy[i]) {
            return false;
        }
    }

    return true;
}

// Solicita recursos para un proceso
void requestResources(int process, int request[NUM_RESOURCES]) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (request[i] > need[process][i] || request[i] > available[i]) {
            return; // La solicitud excede la necesidad máxima o los recursos disponibles
        }
    }

    // Intenta asignar recursos si es seguro
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }

    if (!isSafe(process)) {
        // Si no es seguro, revierte la asignación
        for (int i = 0; i < NUM_RESOURCES; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
    }
}

// Libera recursos asignados a un proceso
void releaseResources(int process, int release[NUM_RESOURCES]) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] += release[i];
        allocation[process][i] -= release[i];
        need[process][i] += release[i];
    }
}

// Muestra el estado actual de la asignación de recursos
void printState() {
    printf("Recursos disponibles: ");
    for (int i = 0; i < NUM_RESOURCES; i++) {
        printf("%d ", available[i]);
    }
    printf("\n");

    printf("Proceso | Máximo | Asignado | Necesidad\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("%7d | ", i);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            printf("%2d ", maximum[i][j]);
        }
        printf("| ");
        for (int j = 0; j < NUM_RESOURCES; j++) {
            printf("%2d ", allocation[i][j]);
        }
        printf("| ");
        for (int j = 0; j < NUM_RESOURCES; j++) {
            printf("%2d ", need[i][j]);
        }
        printf("\n");
    }
}

int main() {
    initialize();
    printState();

    int process = 0;
    int request[NUM_RESOURCES] = {2, 1, 1, 0}; // Proceso 0 solicita recursos

    printf("Proceso %d solicita recursos: ", process);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        printf("%d ", request[i]);
    }
    printf("\n");

    requestResources(process, request);
    printState();

    // Liberar recursos
    int release[NUM_RESOURCES] = {1, 0, 0, 0}; // Proceso 0 libera recursos
    printf("Proceso %d libera recursos: ", process);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        printf("%d ", release[i]);
    }
    printf("\n");

    releaseResources(process, release);
    printState();

    return 0;
}
