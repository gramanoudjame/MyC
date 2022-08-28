/*
 deduplicateDoublyLinkedList.c
 
 Modify the singly linked list to be a doubly linked list.
 Now write a routine that removes all duplicate data in the doubly linked list.
 The data will be integers generated at random from [0,49].
 Initially have a list of 200 elements.
 
 Now do this in one of two ways.
 Sort the list by its data field.
 Remove adjacent elements of the sorted list with the same value.
 Or, take the first element—search the remainder of the list for elements with the same data and remove them.
 Then take the second element and do the same.
 Repeat until only one element is left.
 
 Created by Ganèche Ramanoudjame on 27/08/2022.
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#define INT_FORMAT "%02d "
#define LIST_SIZE 200
#define RAND_RANGE 50
#define PRINT_ROW 5

// Datatype for a chained list
typedef struct list{
    int data;
    struct list *next;
    struct list *previous;
}list;

// Prototypes of funtions
int isEmpty(list*);
list * create(int);
list * addToFront(int, list*);
void print(char*, list*);
list * sortAndDeduplicate(list*);

// Check if list is empty
int isEmpty(list *l){
    return (l == NULL);
}

// Create list
list * create(int data){
    list *head = malloc(sizeof(list));
    head->data = data;
    head->next = NULL;
    head->previous = NULL;
    return head;
}

// Add a new item in front of list
list * addToFront(int data, list *head){
    list *newHead = create(data);
    newHead->next = head;
    if (!isEmpty(head)){
        head->previous = newHead;
    }
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

// Sort list using bubble sort algorithm and remove duplicate
list * sortAndDeduplicate(list *head){
    //return empty list and single element list
    if (isEmpty(head) || isEmpty(head->next)){return head;}
    list *end = NULL, *preHead = addToFront(0, head); // dummy before head
    while(end != head){
        list *bubble = head, *preBubble = preHead;
        while (end != bubble->next){
            if(bubble->data == bubble->next->data){// remove duplicate
                free(bubble->next);
                bubble->next = bubble->next->next;
                if (!isEmpty(bubble->next->next)){
                    bubble->next->next->previous = bubble;
                }
            }
            else if(bubble->data > bubble->next->data){
                head = (head == bubble) ? head->next : head; // bubble is head
                preBubble->next = bubble->next; // swap with next
                bubble->next->previous = preBubble;
                bubble->next = preBubble->next->next;
                if(!isEmpty(preBubble->next->next)){ // not the end
                    preBubble->next->next->previous = bubble;
                }
                preBubble->next->next = bubble;
                bubble->previous = preBubble->next;
            }else{
                bubble = bubble->next; // point bubble to next item
            }
            preBubble = bubble->previous;
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
    print("Unsorted with duplicates : \n", head);
    head = sortAndDeduplicate(head);
    print("Sorted and deduplicated   : \n", head);
    return 0;
}
