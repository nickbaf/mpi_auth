/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Nikolaos Mamais(2371),Nikolaos Bafatakis(2383),Panagiotis Maroulidis(2431)
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

int main(int argc, char** argv) {

    FILE *bin_file;
    int num = 15000000;
    int k;
    char temp[100];
    int size, rank;
    int j = 0;
    MPI_Init(&argc, &argv);
    int c2;
    int counter = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if ((bin_file = fopen("datafile"/*argv[1]*/, "r+")) != NULL) {
        int num1;
        clock_gettime(CLOCK_MONOTONIC, &start);
        int rank_set = (num / size) * rank;
        fseek(bin_file, (num / size) * rank * 31, SEEK_SET);
        printf("\n rank=%d starts at:%d at pos %d", rank, rank_set, (num / size) * rank);
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
    printf("\nrank %d counted  %d", rank, counter);
    MPI_Reduce(&counter, &c2, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        fclose(bin_file);
    }
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
    if(rank==0){
    printf("Time: %ld.%09ld secs \n", timeElapsed_s, timeElapsed_n);
    }
    return (EXIT_SUCCESS);
}


