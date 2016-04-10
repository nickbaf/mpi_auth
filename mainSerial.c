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
#define upper 30
#define lower 12
/*
 * 
 */
struct timespec start, end;


int main(int argc, char** argv) {

    FILE *bin_file;
    int num = 15000000;
    int counter = 0;
    float **data = (float **) malloc(num * sizeof (float *));

    int k;
    int j;

    for (k = 0; k < num; k++) {
        // printf("%d\n",i);
        data[k] = (float *) malloc(3 * sizeof (float));
    }
    char temp[100];
    clock_gettime(CLOCK_MONOTONIC, &start);
    if ((bin_file = fopen("datafile"/*argv[1]*/, "r+")) != NULL) {
        float num1,num2,num3;
        char temp[100];
        for (j = 0; j < 15000000; j++) {
             fread(temp, 2, 1, bin_file);
             fseek(bin_file, 8, SEEK_CUR);
             num1=atoi(temp);
            if(num1<12.0 || num1 >30.0){
                fseek(bin_file, 29, SEEK_CUR);
                continue;
            }
              fread(temp, 2, 1, bin_file);
             fseek(bin_file, 8, SEEK_CUR);
             num2=atoi(temp);
                if(num2<12.0 || num2 >30.0){
                    fseek(bin_file, 19, SEEK_CUR);
                continue;
            }
              fread(temp, 2, 1, bin_file);
             fseek(bin_file, 8, SEEK_CUR);
             num3=atoi(temp);
            if(num3<12.0 || num3 >30.0){
                fseek(bin_file, 9, SEEK_CUR);
                continue;
            }
             fseek(bin_file, 9, SEEK_CUR);
            
                counter++;
        }
    } else {
        printf("Error opening the File");
    }
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


