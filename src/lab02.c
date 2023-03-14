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


/**
 * It creates a matrix of size n x n
 *
 * @param n the number of rows and columns in the matrix
 *
 * @return A pointer to a pointer to a float.
 */
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

/**
 * It frees the memory allocated for the matrix.
 *
 * @param matx The matrix to be destroyed.
 */
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

/**
 * It prints a matrix of size n x n
 *
 * @param matx The matrix to be printed
 * @param n the number of rows and columns in the matrix
 */
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

/**
 * This function populates the matrix with random numbers. It will only populate on coordinates that is divisible by 10, including 0,0
 *
 * @param matx the matrix to be populated
 * @param n the size of the matrix
 */
void populateMatx(float **matx, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if ((i % 10 == 0) && (j % 10 == 0))
                matx[i][j] = (rand() % (1000));
}

/**
 * It takes a 2D array of floats, and interpolates the values in between the points that are already
 * defined
 *
 * @param M The matrix of elevations
 * @param n the size of the matrix
 */
void terrain_inter(float **M, int n)
{
    for (int i = 0; i < n; i++)
    {
        int min_x = getMin(i);
        int max_x = getMax(i);
        for (int j = 0; j < n; j++)
        {
            if (!((i % 10 == 0) && (j % 10 == 0)))
            {
                int min_y = getMin(j);
                int max_y = getMax(j);

                int area_d = (abs(min_x - i) * abs(min_y - j));
                int area_c = (abs(max_x - i) * abs(min_y - j));
                int area_b = (abs(min_x - i) * abs(max_y - j));
                int area_a = (abs(max_x - i) * abs(max_y - j));

                float elev_a = M[min_x][min_y];
                float elev_b = M[max_x][min_y];
                float elev_c = M[min_x][max_y];
                float elev_d = M[max_x][max_y];

                float elevation = ((area_a * elev_a) + (area_b * elev_b) + (area_c * elev_c) + (area_d * elev_d)) / (float)(area_a + area_b + area_c + area_d);

                M[i][j] = elevation;
            }
        }
    }
}


/**
 * It creates a matrix of size n, populates it with random values, and then calls the terrain_inter
 * function.
 * This benchmark is designed to output the necessary information needed for the exercise lab report
 * 
 * @param n The size of the matrix.
 */
void run_benchmark(int n)
{
    srand(time(NULL));
    n++;
    clock_t t;

    float **matx = createMatx(n);
    populateMatx(matx, n);

    t = clock();
    terrain_inter(matx, n);
    t = clock() - t;

    double time_taken = ((double)t) / CLOCKS_PER_SEC;

    // Comment out this code below if you want the program to print the matrices interpolated
    // printMatx(matx, n);

    printf("Matrix size: %d | Time Elapsed: %.5f seconds\n", n - 1, time_taken);

    destroyMatx(matx);
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

    // ==============================================================
    // This part of the code is for the benchmarking for lab01-pt2
    // for (int i = 0; i < 3; i++)
    // {
    //     printf("Run # %d\n", i+1);
    //     run_benchmark(100);
    //     run_benchmark(200);
    //     run_benchmark(300);
    //     run_benchmark(400);
    //     run_benchmark(500);
    //     run_benchmark(600);
    //     run_benchmark(700);
    //     run_benchmark(800);
    //     run_benchmark(900);
    //     run_benchmark(1000);
    //     run_benchmark(2000);
    //     run_benchmark(3000);
    //     run_benchmark(4000);
    //     run_benchmark(8000);
    //     run_benchmark(16000);
    //     run_benchmark(20000);
    // }

    return 0;
}