#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20
#define NUM_THREADS 10

int matA[MAX][MAX];
int matB[MAX][MAX];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
 * Thread i computes rows:
 * i=0 -> rows 0,1
 * i=1 -> rows 2,3
 * ...
 * i=9 -> rows 18,19
 */
void* computeSum(void* args) {
    int tid = *((int*)args);
    free(args);

    int startRow = tid * 2;
    int endRow = startRow + 2;

    for(int i = startRow; i < endRow; i++) {
        for(int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }

    return NULL;
}

void* computeDiff(void* args) {
    int tid = *((int*)args);
    free(args);

    int startRow = tid * 2;
    int endRow = startRow + 2;

    for(int i = startRow; i < endRow; i++) {
        for(int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }

    return NULL;
}

void* computeProduct(void* args) {
    int tid = *((int*)args);
    free(args);

    int startRow = tid * 2;
    int endRow = startRow + 2;

    for(int i = startRow; i < endRow; i++) {
        for(int j = 0; j < MAX; j++) {

            matProductResult[i][j] = 0;

            for(int k = 0; k < MAX; k++) {
                matProductResult[i][j] +=
                    matA[i][k] * matB[k][j];
            }
        }
    }

    return NULL;
}

int main() {
    srand(time(0));

    fillMatrix(matA);
    fillMatrix(matB);

    printf("Matrix A:\n");
    printMatrix(matA);

    printf("Matrix B:\n");
    printMatrix(matB);

    pthread_t threads[NUM_THREADS];

    /* SUM */
    for(int i = 0; i < NUM_THREADS; i++) {
        int *tid = malloc(sizeof(int));
        *tid = i;

        pthread_create(
            &threads[i],
            NULL,
            computeSum,
            tid
        );
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* DIFFERENCE */
    for(int i = 0; i < NUM_THREADS; i++) {
        int *tid = malloc(sizeof(int));
        *tid = i;

        pthread_create(
            &threads[i],
            NULL,
            computeDiff,
            tid
        );
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* PRODUCT */
    for(int i = 0; i < NUM_THREADS; i++) {
        int *tid = malloc(sizeof(int));
        *tid = i;

        pthread_create(
            &threads[i],
            NULL,
            computeProduct,
            tid
        );
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Results:\n");

    printf("Sum:\n");
    printMatrix(matSumResult);

    printf("Difference:\n");
    printMatrix(matDiffResult);

    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}