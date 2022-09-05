//
//  binaryTreeSort.c
// Open and read a file of integer pairs into an array that is created with the first integer telling you how many to read.
// So  4 9  11  4  5    would mean create an integer array size 4 and read into data[].
// Write a routine that places these values into a binary tree structure. Then walk the tree “inorder” and print these values to the screen.
// Submit your work in a text file.
//  Created by Ganèche Ramanoudjame on 04/09/2022.
//

#include <stdio.h>
#include <stdlib.h>

// Type binary tree
typedef struct btree{
    int value;
    struct btree *left;
    struct btree *right;
}btree;

// Prototypes
FILE * openFile(const char *, char *);
btree * createBtree(const int);
void insert(btree**, const int);
btree * arrayToBTree(const int *, const int);
void printfInOrder(btree *);

// Returns a FILE given it's path and the mode of opening. Exits if file cannot be opened.
FILE * openFile(const char path[], char mode[]){
    FILE *result = fopen(path, mode);
    if(result == NULL){
        printf("Cannot open file : %s\n", path);
        exit(1);
    }
    return result;
}

// Create a new btree node
btree * createBtree(const int v){
    btree *b = malloc(sizeof(btree));
    b->value = v;
    b->left = NULL;
    b->right = NULL;
    return b;
}

// Printf btree inorder
void printfInOrder(btree *b){
    if(b != NULL){
        printfInOrder(b->left);
        printf("%d ", b->value);
        printfInOrder(b->right);
    }
}

// Insert a btree node given adress of root
void insert(btree **root, const int value){
    btree *b = *root;
    btree *bParent = NULL;
    // look for the empty leaf
    while (b != NULL){
        bParent = b;
        b = (value < b->value) ? b->left : b->right;
    }
    // then create the new leaf
    if(bParent == NULL){
        *root = createBtree(value);
    } else if(value < bParent->value){
        bParent->left = createBtree(value);
    }else{
        bParent->right = createBtree(value);
    }
}

// Array to bTree
btree * arrayToBTree(const int a[], const int size){
    btree *b = NULL;
    for(int i = 0; i<size; i++){
        insert(&b, a[i]);
    }
    return b;
}

int main(int argc, const char * argv[]) {
    // check that 1 argument has been provided as intput file name
    if(argc!=2){
        printf("Wrong number of arguments\n");
        exit(1);
    }
    
    FILE *ifp = openFile(argv[1], "r");
    
    int size;
    fscanf(ifp, "%d", &size);     // scan first integer as array size
    int data[size];
    for(int i=0; i<size; i++){     // then loop to scan integers
        fscanf(ifp, "%d", &(data[i]));
    }
    
    printfInOrder(arrayToBTree(data, size));
    printf("\n");
    
    return 0;
}
