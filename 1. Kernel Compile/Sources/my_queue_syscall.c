/*
 *  linux/kernel/my_queue_syscall.c
 *
 *  2021-04-09  Jangyoung Kim - Korea Univ.
 *		Add custom syscall for queueing
 *		oslab_enqueue(335); enqueue 
 *		oslab_dequeue(336); dequeue
 */

#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#define MAX_LENGTH 1000

// Define Node
typedef struct Node {
    int data;
    struct Node *next;
}Node;

// Define Queue
typedef struct Queue 
{
    Node *front; 
    Node *rear; 
    int count;
    int point_num;
}Queue;

// Declare helper functions
int IsEmpty(Queue *queue);
int IsAlreadyExist(Queue *queue, int new_input);
void PrintQueue(Queue *queue);


/** Main Routine **/
/* Declare Global Queue */
struct Queue queue = {NULL,NULL,0, 0};
struct Node n[MAX_LENGTH] = {0, };

/* Enqueue Syscall */
SYSCALL_DEFINE1(oslab_enqueue, int, a) {
    Node *now = n; 
    now += sizeof(Node)*queue.point_num;// Create Node & Node initialize to enqueue
    now->data = a;
    now->next = NULL;

    if (IsEmpty(&queue)) {	              // if queue is empty
        queue.front = now;                    // set front to new node   
    }
    else {
        if (IsAlreadyExist(&queue, a)) {      // if input value is already exits -> break
	    printk(KERN_INFO "[Error] - Already existing value \n");
	    return;
        }
        queue.rear->next = now;		      // else rear->next to new node
    }
    queue.rear = now;   
    queue.count++;
    queue.point_num++;

    printk(KERN_INFO "[System call] oslab_enqueue(); -----\n");
    PrintQueue(&queue);
}

/* Dequeue Syscall */
SYSCALL_DEFINE0(oslab_dequeue) {
    int re = 0;
    Node *now;
    if (IsEmpty(&queue)) {		// if queue is empty -> print "EMPTY QUEUE"
	printk(KERN_INFO "[Error] - EMPTY QUEUE--------------\n");
        return -2;
    }
    now = queue.front;
    re = now->data; 			// Return value setting
    queue.front = now->next;
    queue.count--;			// discounting
    
    printk(KERN_INFO "[System call] oslab_dequeue(); ------\n");
    PrintQueue(&queue);
    return re;
}




/** Helper Functions **/


/* 1. IsEmpty: check whether queue is empty or not 
 * by checking queue.count == 0
 */
int IsEmpty(Queue *queue) {
    return queue->count == 0; // True or False
}


/* 2. IsAlreadyExist:
 * check whether new_input already exists or not 
 * by searhcing linked list
 */
int IsAlreadyExist(Queue *queue, int new_input) {
    Node *now = queue->front;
    if (now == NULL) {
        return 0;
    }
    while(1) {
	if (now->data == new_input){
	    return 1;
	}
	
        if (now->next == NULL){
	    break;
	}
	now = now->next;
    }
    return 0;
}


/* 3. PrintQueue:
 * printing values exist in global Queue
 */
void PrintQueue(Queue *queue) {
    Node *now = queue->front;
    if (now == NULL) {
	printk("Queue Front------------------------\n");
	printk("Queue Rear---------------------\n");
        return;
    }
    printk("Queue Front------------------------\n");
    while(1) {
	printk("%d\n", now->data);
        if (now->next == NULL){
	    printk("Queue Rear---------------------\n");
	    break;
	}
	now = now->next;
    }
    return;
}

