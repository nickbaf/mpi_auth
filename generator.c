/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Nikolaos Mamais(2371),Nikolaos Bafatakis(2383),Panagiotis Maroylidis(2431)
 *
 * Created on 1 Μαρτίου 2016, 4:48 μμ
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
 * 
 */
int main(int argc, char** argv) {
    printf("%s\n", argv[0]);
    int coordinate_index = atoi(argv[2]);
    int utime;
    long int ltime;
    int i;
    FILE *bin_file;
    ltime = time(NULL);
    utime = (unsigned int) ltime / 2;
    srand(utime);
    unsigned int counter=0;
    if ((bin_file = fopen(argv[1], "w")) != NULL) {
        for (i = 0; i < coordinate_index; i++) {
            char temp[3][20];
            char flag=1;
            for (int j = 0; j < 3; j++) {
                float a = (float) 34 * rand() / (RAND_MAX - 1);
                sprintf(temp[j], "%f", a);
                if (a < 10.0) {
                    fprintf(bin_file, "0%s ", temp[j]);
                } else {
                    fprintf(bin_file, "%s ", temp[j]);
                }
                if(a<12.0 || a>30.0){
					flag=0;
				}
				
            }
            if(flag) counter++;
            fprintf(bin_file, "\n");
        }
        fclose(bin_file);
    } else {
        printf("Error opening the File");
    }
    printf("%d",counter);
    return (EXIT_SUCCESS);
}
