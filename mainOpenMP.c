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
    int counter = 0;
    float **data = (float **) malloc(num * sizeof (float *));

    int k;
    int j;
    //#pragma omp parallel private(k,j)//reduction(+:counter)
    // {


    //#pragma omp parallel for private(k)
    for (k = 0; k < num; k++) {
        // printf("%d\n",i);
        data[k] = (float *) malloc(3 * sizeof (float));
    }
    char temp[100];
    clock_gettime(CLOCK_MONOTONIC, &start);
#pragma omp parallel private(bin_file,temp) //reduction(+:counter)
    if ((bin_file = fopen("datafile"/*argv[1]*/, "r+")) != NULL) {
        //printf("hjfdnk\n");
        //#pragma omp parallel for private(j)
       //fseek(bin_file,(j)*31 ,SEEK_SET );// fseek(bin_file, SEEK_SET, 0);
        int num1;
#pragma omp for private(j,num1) reduction(+:counter) 
      //  fseek(bin_file, SEEK_SET, 0);
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

    /*  int threads;
      int i = 0;

      //#pragma omp barrier
  #pragma omp parallel for default(shared) private(i) reduction(+:counter)
      for (i = 0; i < num; i++) {

          if (check(data[i])) {

              counter = counter + 1;
          }
      }

      //}*/
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


