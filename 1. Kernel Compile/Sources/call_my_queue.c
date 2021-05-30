/*
 *  call_my_queue.c
 *
 *  2021-04-10  Jangyoung Kim - Korea Univ.
 *		Application using my_queue_syscall()
 */

#include <stdio.h>
#include <unistd.h>

#define my_queue_enqueue 335
#define my_queue_dequeue 336

int main(){
    int a;
    for(int i=1; i<=3; i++){
	a = syscall(my_queue_enqueue, i);
	printf("Enqueue : %d\n", i);
	
    }
    a = syscall(my_queue_enqueue, 3);
    printf("Enqueue : %d\n", 3);


    for(int i=1; i<=3; i++){
	a = syscall(my_queue_dequeue);
	printf("Dequeue : %d\n", a);
    }
}



