/*
 bubbleSortLinkedList.c

 Use the linear linked list code to store a randomly generated set of 100 integers.
 Now write a routine that will rearrange the list in sorted order of these values.
 Note you might want to use bubble sort to do this.
 Print these values in rows of 5 on the screen.
 How will this work?
 Compare two adjacent list elements and if they are out of order swap them.
 After a first pass the largest element will have bubbled to the end of the list.
 Each pass can look at one less element as the end of a list stays sorted.
 
 Created by Ganèche Ramanoudjame on 25/08/2022.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#define INT_FORMAT "%04d "
#define LIST_SIZE 100
#define RAND_RANGE 10000
#define PRINT_ROW 5

// Datatype for a chained list
typedef struct list{
    int data;
    struct list *next;
}list;

// Prototypes of funtions
int isEmpty(list*);
list * create(int);
list * addToFront(int, list*);
void print(char*, list*);
list * bubbleSort(list*);

// Check if list is empty
int isEmpty(list *l){
    return (l == NULL);
}

// Create list
list * create(int data){
    list *head = malloc(sizeof(list));
    head->data = data;
    head->next = NULL;
    return head;
}

// Add a new item in front of list
list * addToFront(int data, list *head){
    list *newHead = create(data);
    newHead->next = head;
    return newHead;
}

// Print list in rows
void print(char *title, list *head){
    printf("%s", title);
    int i = 0;
    while(!isEmpty(head)){
        printf(INT_FORMAT, head->data);
        if (++i%PRINT_ROW == 0) {
            printf("\n");
        }
        head = head->next;
    }
    printf("\n\n");
}

// Sort list using bubble sort algorithm
list * bubbleSort(list *head){
    //return empty list and single element list
    if (isEmpty(head) || isEmpty(head->next)){return head;}
    list *end = NULL, *preHead = addToFront(0, head); // dummy before head
    while(end != head){
        list *bubble = head, *preBubble = preHead;
        while (end != bubble->next){
            if(bubble->data > bubble->next->data){
                head = (head == bubble) ? head->next : head; // bubble is head
                preBubble->next = bubble->next; // swap with next
                bubble->next = preBubble->next->next;
                preBubble->next->next = bubble;
                preBubble = preBubble->next;
            }else{
                preBubble = bubble; // point bubble to next item
                bubble = bubble->next;
            }
        }
        end = bubble;
    }
    free(preHead);
    return head;
}

int main(int argc, const char *argv[]) {
    // Initialisation
    srand((int)time(NULL));
    list *head = NULL;
    for(int i=0; i<LIST_SIZE; i++){
        head = addToFront(rand()%RAND_RANGE, head);
    }
    print("Unsorted : \n", head);
    head = bubbleSort(head);
    print("Sorted   : \n", head);
    return 0;
}
