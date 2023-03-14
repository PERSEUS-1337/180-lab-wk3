// CMSC 180 Lab 01 Part 02
// Author: Aron Resty Ramillano | 2020-01721
// Section: T3L

// This is an attempt to optimize my python code and turn it into C code that is much faster

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h> //for threads

// Resources:
// https://www.geeksforgeeks.org/multidimensional-arrays-c-cpp/
// https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
// https://stackoverflow.com/questions/5201708/how-to-return-a-2d-array-from-a-function-in-c

#define NUM_THREADS 12

typedef struct
{
    float **M;
    int n;
    int start;
    int end;
} ThreadArgs;

float **createMatx(int n)
{
    float *values = malloc(n * sizeof(float));
    float **rows = malloc(n * sizeof(float *));
    for (int i = 0; i < n; ++i)
    {
        rows[i] = malloc(sizeof(float) * n);
    }
    return rows;
}

void destroyMatx(float **matx)
{
    free(*matx);
    free(matx);
}

int getMin(int n)
{
    int min = 0;

    if (n > 1)
        min = floor((n - 1) / 10) * 10;

    return min;
}

int getMax(int n)
{
    int max = 10;

    if (n > 0)
        max = ceil(n / 10.0) * 10;

    return max;
}

void printMatx(float **matx, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%.3f ", matx[i][j]);
        }
        printf("\n");
    }
}

void populateMatx(float **matx, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if ((i % 10 == 0) && (j % 10 == 0))
                matx[i][j] = (rand() % (1000));
}

void *thread_func(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;

    for (int i = args->start; i < args->end; i++)
    {
        int min_x = getMin(i);
        int max_x = getMax(i);
        for (int j = 0; j < args->n; j++)
        {
            if (!((i % 10 == 0) && (j % 10 == 0)))
            {
                int min_y = getMin(j);
                int max_y = getMax(j);

                int area_d = (abs(min_x - i) * abs(min_y - j));
                int area_c = (abs(max_x - i) * abs(min_y - j));
                int area_b = (abs(min_x - i) * abs(max_y - j));
                int area_a = (abs(max_x - i) * abs(max_y - j));

                float elev_a = args->M[min_x][min_y];
                float elev_b = args->M[max_x][min_y];
                float elev_c = args->M[min_x][max_y];
                float elev_d = args->M[max_x][max_y];

                float elevation = ((area_a * elev_a) + (area_b * elev_b) + (area_c * elev_c) + (area_d * elev_d)) / (float)(area_a + area_b + area_c + area_d);

                args->M[i][j] = elevation;
            }
        }
    }

    return NULL;
}

void terrain_inter(float **M, int n)
{
    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];

    int chunk_size = n / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        args[i].M = M;
        args[i].n = n;
        args[i].start = i * chunk_size;
        args[i].end = (i + 1) * chunk_size;

        pthread_create(&threads[i], NULL, thread_func, &args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

int main()
{
    // This first part of the code is for using the code with inputs

    int n = 0;
    printf("Enter n divisible by 10: ");
    int check = scanf("%d", &n);

    while (!check || n <= 0 || n % 10 != 0)
    {
        printf("Wrong Input! Try again: ");
        check = scanf("%d", &n);
    }

    
    // +1 to include the actual 10th coordinate
    n++;

    // Reinitializes the randomizer for C
    srand(time(NULL));

    float **matx = createMatx(n);
    populateMatx(matx, n);

    // The actual interpolation
    clock_t t;
    t = clock();
    terrain_inter(matx, n);
    t = clock() - t;

    double time_taken = ((double)t) / CLOCKS_PER_SEC;

    // Printing the matrix in a clean way
    // printMatx(matx, n);
    printf("\nElapsed time: %.5f seconds\n", time_taken);

    destroyMatx(matx);

    return 0;
}