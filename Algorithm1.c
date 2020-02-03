#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
/* Macro that controls the matrix dimensions */
int MATRIX_SIZE = 0;

/* Macro for addressing a normal array as if it were two dimensional */
#define array(arr, i, j) arr[(int)MATRIX_SIZE * (int)i + (int)j]
void fill_matrix(int *matrix);
void print_matrix(int *matrix, int print_width);
int *matrix_page(int *matrix, unsigned long m_size);
void matrix_unmap(int *matrix, unsigned long m_size);
__attribute__((noreturn)) void row_multiply(void *row_args);
/* Integer pointers for each matrix */
static int *matrix_a, *matrix_b, *matrix_c;
/* Argument struct for each thread */
typedef struct arg_struct
{
    int *a;
    int *b;
    int *c;
    int row;
} thr_args;
/* Fill the given matrix with an integer from 1 to 10 */
void fill_matrix(int *matrix)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            array(matrix, i, j) = rand() % 10 + 1;
        }
    }
    return;
}
/* Print the given matrix */
void print_matrix(int *matrix, int print_width)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            printf("[%*d]", print_width, array(matrix, i, j));
        }
        printf("\n");
    }
    printf("\n");
    return;
}
/* Maps the given matrix into a memory page using mmap() */
int *matrix_page(int *matrix, unsigned long m_size)
{
    matrix = mmap(0, m_size, PROT_READ | PROT_WRITE,
                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    /* If mmap() failed, exit! */
    if (matrix == (void *)-1)
    {
        exit(EXIT_FAILURE);
    }
    memset((void *)matrix, 0, m_size);
    return matrix;
}
/* Unmaps the given matrix from its memory page */
void matrix_unmap(int *matrix, unsigned long m_size)
{
    /* If munmap() failed, exit! */
    if (munmap(matrix, m_size) == -1)
    {
        exit(EXIT_FAILURE);
    }
}
/* Calculate all indices for the given row */
__attribute__((noreturn)) void row_multiply(void *row_args)
{
    thr_args *args = (thr_args *)row_args;
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            int add = array(args->a, args->row, j) * array(args->b, j, i);
            array(args->c, args->row, i) += add;
        }
    }
    pthread_exit(0);
}
int main()
{
    FILE *sizes;
    FILE *time;
    sizes = fopen("Matrix Sizes.txt" , "w+");
    time = fopen("Execution Time.txt", "w+");
    for (int ini = 0; ini < 100; ini++)
    {
	MATRIX_SIZE += 10;

        /* Calculate the memory size of the matrices */
        unsigned long m_size = sizeof(int) * (unsigned long)(MATRIX_SIZE * MATRIX_SIZE);
        /* Map matrix_a, matrix_b, and matrix_c into a memory page */
        matrix_a = matrix_page(matrix_a, m_size);
        matrix_b = matrix_page(matrix_b, m_size);
        matrix_c = matrix_page(matrix_c, m_size);
        /* Fill both matrices with random integers 1-10 */
        fill_matrix(matrix_a);
        fill_matrix(matrix_b);

        /* Allocate arrays for thread data */
        pthread_t *thrs;
        thr_args *args;
        if ((thrs = malloc(sizeof(pthread_t) * (unsigned long)MATRIX_SIZE)) == NULL ||
            (args = malloc(sizeof(thr_args) * (unsigned long)MATRIX_SIZE)) == NULL)
        {
            exit(EXIT_FAILURE);
        }
        /* Create threads 0, 1, ..., N-1, and give them a struct with their data */
        clock_t startTime = clock();
        for (int i = 0; i < MATRIX_SIZE; i++)
        {
            args[i] = (thr_args){
                .a = matrix_a,
                .b = matrix_b,
                .c = matrix_c,
                .row = i};
            pthread_create(&thrs[i], NULL, (void *)&row_multiply, (void *)&args[i]);
        }
        /* Collect each thread */
        for (int j = 0; j < MATRIX_SIZE; j++)
            pthread_join(thrs[j], NULL);
	clock_t termination = clock();
        /* Free resources allocated for each thread */
        if (thrs != NULL)
        {
            free(thrs);
            thrs = NULL;
        }
        if (args != NULL)
        {
            free(args);
            args = NULL;
        }
       
    /* Give back the allocated memory pages */
        matrix_unmap(matrix_a, m_size);
        matrix_unmap(matrix_b, m_size);
        matrix_unmap(matrix_c, m_size);
        
        double executionTime = (termination - startTime) * 1000 / CLOCKS_PER_SEC;
        fprintf(sizes, "%d", MATRIX_SIZE);
        fprintf(sizes, "\n");
        fprintf(time, "%f", executionTime);
        fprintf(time, "\n");
    }
}
