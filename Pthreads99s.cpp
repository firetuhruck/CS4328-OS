/*
Name: Michael Luster
Date: 3/15/2016
Instructor: Zong
Program: 1.2a
Description: Program will implement the given sequential code
using pthreads to create an array size and amount of threads created
be accepted as a single positive integer
The final output will display the time taken to find the amount of 99s
in an array of x size, in both series and parallel code

Written in C++

*/
#include <stdio.h>
#include <omp.h>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>


using namespace std;

typedef struct
{
	int t_id,
        num,
        start,
        end;
}tData;


int SIZE;
int NUM_THREADS;
int *Array;
//count99s
void *count99s(void *para)
{
	tData *td = (tData*) para;
	for (int i = td->start; i < td->end; i++)
	   if(Array[i] == 99)
		td->num++;
}


int parallelFind(int arr[])
{
    pthread_t workers[NUM_THREADS];
    tData data[NUM_THREADS];
    int workSize = SIZE/NUM_THREADS;
	int num = 0;
	double pStart,
            pEnd;

	pStart = omp_get_wtime();
        for(int i=0; i<NUM_THREADS; i++)
        {
                pthread_t *thread = &workers[i];
                tData *p = &data[i];
                p->t_id = i;
                p->start = i * workSize;
                p->end = ((i + 1) * workSize) - 1;
		p->num = 0;
                pthread_create(thread, NULL, count99s, (void*)p);
        }

        for(int i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(workers[i], NULL);
	    num += data[i].num;
        }
	pEnd = omp_get_wtime();

	printf("The parallel code indicates that %d 99's in the array.\n", num);
	printf("The parallel code used %f seconds to complete the execution\n\n", pEnd-pStart);

        return 0;
}

int serialFind(int arr[])
{
	double sStart,
            sEnd;
	int cnt = 0;
	sStart = omp_get_wtime();

	for(int i = 0; i < SIZE; i++)
	   if(arr[i] == 99)
	      cnt++;
	sEnd = omp_get_wtime();

	printf("The serial code indicates that %d 99's in the array.\n", cnt);
	printf("The serial code used %f seconds to complete the execution\n\n", sEnd-sStart);
}

int main (){
	//prompts
	cout << "size of array: ";
	cin >> SIZE;
	if(SIZE == -1)
        return -1;

	cout << "number of threads (-1 to quit) : \n";
	cin >> NUM_THREADS;
	if(SIZE == -1)
	return -1;

	//dynamically allocated as mentioned in class
	int *arr = new int[SIZE];
	//time calculators
	srand((unsigned)time(NULL));
	for(int i  = 0; i < SIZE; i++)
	   arr[i] = rand() % 100;

	Array = arr;
	serialFind(Array);
	parallelFind(Array);

	delete[] arr;
        return 0;
}

