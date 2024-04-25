#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define MATRIX_SIZE 3
#define NUM_THREADS 3

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];

// creation of calc variable and mut 
int calcCount;          
pthread_mutex_t mut;

// structure for thread data
typedef struct {
    int row;
    int col;
} thread_data_t;

// function to multiply matrices
void *multiply(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;

    // multiply the row of matrix A by the column of matrix B to get the element of the result matrix
    for (int i = 0; i < MATRIX_SIZE; i++) {
        resultMatrix[data->row][data->col] += matrixA[data->row][i] * matrixB[i][data->col];
        pthread_mutex_lock(&mut); // lock mutex
        calcCount++;
        pthread_mutex_unlock(&mut); // unlock mutex
    }
   
    pthread_exit(NULL);
}

// main function
int main() {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    pthread_mutex_init(&mut, 0); //mutex initialization

    // initialize matrices A and B
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = rand() % 10;
            matrixB[i][j] = rand() % 10;
        }
    }

    // create threads to compute the result matrix
    int thread_count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            thread_data[thread_count].row = i;
            thread_data[thread_count].col = j;
            pthread_create(&threads[thread_count], NULL, multiply, (void *)&thread_data[thread_count]);
            thread_count++;

            if (thread_count >= NUM_THREADS)
                thread_count = 0;
        }
    }

    // wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // print the result matrix
    printf("Result Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }

    // print the calculation count
    printf( "Calculation Count: ");
    printf( "%d", calcCount);
    printf("\n");

    // destroy mutex
    return 0;
}
