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
#define upper 30
#define lower 12
/*
 * 
 */
struct timespec start, end;



int main(int argc, char** argv) {

    FILE *bin_file;
    //kanw if gia na vrw ton arithmo
    int num = 15000000;
    int counter = 0;
    float **data = (float **) malloc(num * sizeof (float *));
    int k;
    int j;
    char temp[100];
    clock_gettime(CLOCK_MONOTONIC, &start);
#pragma omp parallel private(bin_file,temp) 
    if ((bin_file = fopen("datafile"/*argv[1]*/, "r+")) != NULL) {
        int num1;


#pragma omp for private(j,num1) reduction(+:counter) 
      
        for (j = 0; j < num; j++) {
  
            // fseek(bin_file,(j)*31 ,SEEK_SET );
            fread(temp, 2, 1, bin_file);
            num1 = atoi(temp);
            if (num1 < 12 || num1 > 30) {
                fseek(bin_file,(j+1)*31, SEEK_SET);

                continue;
            }

            fseek(bin_file, 8, SEEK_CUR);
            fread(temp, 2, 1, bin_file);

            num1 = atoi(temp);

            if (num1 < 12 || num1 > 30) {
                fseek(bin_file,(j+1)*31, SEEK_SET);

                continue;
            }
            fseek(bin_file, 8, SEEK_CUR);
            fread(temp, 2, 1, bin_file);
            num1 = atoi(temp);

            if (num1 < 12 || num1 > 30) {
                fseek(bin_file,(j+1)*31, SEEK_SET);
                continue;
            }
            //fseek(bin_file,9, SEEK_SET);
            fseek(bin_file,(j+1)*31, SEEK_SET);
            counter = counter + 1;

        }
    } else {
        printf("Error opening the File");
    }
   // fclose(bin_file)


    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("\n%d\n", counter);
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


