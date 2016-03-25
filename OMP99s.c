/*
Name: Michael Luster
Date: 3/15/2016
Instructor: Zong
Program: 1.2a
Description: Program will implement the given sequential code
using OMP to create an array size and amount of threads created
be accepted as a single positive integer
The final output will display the time taken to find the amount of 99s
in an array of x size, in both seri0es and parallel code of a given amount of worker threads

*/
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>


int SIZE;
int NUM_THREADS;
int *array;

int main()
{
	omp_set_dynamic(0);

	printf("size of array: ");
	scanf("%d", &SIZE);
	if(SIZE < 1)
	   return -1;
    printf("\nnumber of threads (-1 to quit) : ");
    scanf("%d", &NUM_THREADS);
    if(NUM_THREADS<0)
        return -1;

	array = malloc(SIZE * sizeof(int));
	srand((unsigned)time(NULL));

	for(int i = 0; i < SIZE; i++)
	   array[i] = rand() % 100;

    //serial portion
    double sStart = omp_get_wtime();
    int Scount =0;
	for(int i = 0; i < SIZE; i++)
	   if(array[i] == 99)
		Scount++;
	double sEnd = omp_get_wtime();

		printf("The serial code indicates that %d 99's in the array.\n", Scount);
		printf("The serial code used %f seconds to complete the execution.\n\n",sEnd-sStart);

    //parallel portion
	double pStart = omp_get_wtime();
	int pCount = 0;
	#pragma omp parallel for default(none) shared(SIZE, array) private(i) reduction(+:pCount) num_threads(NUM_THREADS)
	for(int i = 0; i < SIZE; i++)
	{
	   if(array[i] == 99)
		pCount++;
	}
    double pEnd= omp_get_wtime();

	printf("The parallel code indicates that %d 99's in the array\n", pCount);
	printf("The parallel code used %f seconds to complete the execution\n\n", pEnd-pStart);

	return 0;
}

