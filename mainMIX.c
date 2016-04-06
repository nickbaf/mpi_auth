/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: baf
 *
 * Created on March 19, 2016, 1:30 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <mpi.h>
#define upper 30
#define lower 12
/*
 * 
 */
struct timespec start, end;

char check(float s[3]) {
    //#pragma omp parallel 
    if (s[0] < lower || s[0] > upper) {
        return 0;
    } else if (s[1] < lower || s[1] > upper) {
        return 0;
    } else if (s[2] < lower || s[2] > upper) {
        return 0;
    } else
        return 1;
}

int main(int argc, char** argv) {

    FILE *bin_file;
    //kanw if gia na vrw ton arithmo
    int num = 15000000;

    float **data = (float **) malloc(num * sizeof (float *));

    int k;
    //#pragma omp parallel private(k,j)//reduction(+:counter)
    // {


    //#pragma omp parallel for private(k)
    for (k = 0; k < num; k++) {
        // printf("%d\n",i);
        data[k] = (float *) malloc(3 * sizeof (float));
    }
    char temp[100];
    int size, rank;
    int j = 0;
    MPI_Init(&argc, &argv);
    int c2;
    int counter = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#pragma omp parallel private(bin_file,temp) //reduction(+:counter)
    if ((bin_file = fopen("datafile"/*argv[1]*/, "r+")) != NULL) {
        int num1;
        clock_gettime(CLOCK_MONOTONIC, &start);
        int rank_set = (num / size) * rank;
        fseek(bin_file, (num / size) * rank * 31, SEEK_SET);
        printf("\n rank=%d starts at:%d at pos %d", rank, rank_set, (num / size) * rank);
#pragma omp for private(num1,j) reduction(+:counter) 
        for (j = 0; j < (num / size); j++) {
            fread(temp, 2, 1, bin_file);
            num1 = atoi(temp);
            if (num1 < lower || num1 > upper) {
                fseek(bin_file, 29, SEEK_CUR);
                continue;
            }
            // if(rank==1) printf("%d",num1);
            fseek(bin_file, 8, SEEK_CUR);
            fread(temp, 2, 1, bin_file);
            num1 = atoi(temp);
            if (num1 < lower || num1 > upper) {
                fseek(bin_file, 19, SEEK_CUR);
                continue;
            }
            fseek(bin_file, 8, SEEK_CUR);
            fread(temp, 2, 1, bin_file);
            num1 = atoi(temp);
            if (num1 < lower || num1 > upper) {
                fseek(bin_file, 9, SEEK_CUR);
                continue;
            }
            fseek(bin_file, 9, SEEK_CUR);
            counter = counter + 1;
        }
    } else {
        printf("Error opening the File");
    }
    #pragma omp barrier
    printf("\nrank %d counted  %d", rank, counter);
    
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Reduce(&counter, &c2, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   // fclose(bin_file);
    /*  int threads;
      int i = 0;
     * fclose
      //#pragma omp barrier
  #pragma omp parallel for default(shared) private(i) reduction(+:counter)
      for (i = 0; i < num; i++) {

          if (check(data[i])) {

              counter = counter + 1;
          }
      }

      //}*/
    MPI_Finalize();

    clock_gettime(CLOCK_MONOTONIC, &end);
    // printf("\nrank %d counted %d\n", rank, counter);
    if (rank == 0) {
        printf("\n%d\n", c2);
    }
    const int DAS_NANO_SECONDS_IN_SEC = 1000000000;
    long timeElapsed_s = end.tv_sec - start.tv_sec;
    long timeElapsed_n = end.tv_nsec - start.tv_nsec;
    //If we have a negative number in timeElapsed_n , borrow a carry from seconds

    if (timeElapsed_n < 0) {
        timeElapsed_n =
                DAS_NANO_SECONDS_IN_SEC + timeElapsed_n;
        timeElapsed_s--;
    }
    printf("Time: %ld.%09ld secs \n", timeElapsed_s, timeElapsed_n);
    return (EXIT_SUCCESS);
}


