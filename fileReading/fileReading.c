//
//  fileReading.c
//
// Open and read a file of integers into an array that is created with the first integer telling you how many to read.
// So  4  9  11  12  15  would mean create an int array size 4 and read in the remaining 4 values into data[].
// Then compute their average as a double and their max  as an int.  Print all this out neatly to the screen and to an output file named answer-hw3.
//  Created by Ganèche Ramanoudjame on 02/09/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

#define OUTPUT_FILE "answer-hw3" //if launched in an IDE debug, you might need to add absolute path
#define MAX(a,b) ((a)<(b)?(b):(a))
#define FPRINTF(file,args...)\
do {\
    printf(args);\
    fprintf(file,args);\
}while(0)

// Prototypes
FILE * openFile(const char *, char *);
void averageAndMax(int *, int, double *, int *);

// Returns a FILE given it's path and the mode of opening. Exits if file cannot be opened.
FILE * openFile(const char path[], char mode[]){
    FILE *result = fopen(path, mode);
    if(result == NULL){
        printf("Cannot open file : %s\n", path);
        exit(1);
    }
    return result;
}

// Compute average and max with the same loop for the given array
void averageAndMax(int a[], int size, double *average, int *max){
    *average=0;
    *max = INT_MIN;
    for(int i=0; i<size; i++){
        *average += a[i];
        *max = MAX(*max, a[i]);
    }
    *average = *average / size;
}

int main(int argc, const char * argv[]) {
    // check that 1 argument has been provided as intput file name
    if(argc!=2){
        printf("Wrong number of arguments\n");
        exit(1);
    }
    
    // open files
    FILE *ifp = openFile(argv[1], "r");
    FILE *ofp = openFile(OUTPUT_FILE, "w");

    // scan first integer as array size
    int size;
    fscanf(ifp, "%d", &size);
    // then loop to scan integers
    int values[size];
    for(int i=0; i<size; i++){
        fscanf(ifp, "%d", &(values[i]));
    }
    
    // compute average and max
    double average;
    int max;
    averageAndMax(values, size, &average, &max);
    
    // print result both to screen and output file using macro
    FPRINTF(ofp, "array = [");
    for(int i=0; i<size; i++) {
        FPRINTF(ofp, "%d%c", values[i], (i!=size-1)?44:0);
    }
    FPRINTF(ofp, "]\n");
    FPRINTF(ofp, "average = %f \nmax = %d\n", average, max);
    
    // close files
    fclose(ifp);
    fclose(ofp);
    
    return 0;
}
