
/******************************************************************************************************************************************************************************
	
	
						CO-327
						Operating Systems  
						
 Name : Jayathilaka H.A.D.T.T.
 E-No : E/16/156
 
 
 Producer consumer unbounded buffer
 
 The buffer in the given code is implemented using an array and hence it is bounded. Your task is to replace the array 
 with a linked list implementation so that buffer would be unbounded. You will also need to modify the synchronization 
 primitives suite this new situation. 
 
 *******************************************************************************************************************************************************************************/



#include <pthread.h> // posix threads 
#include <stdio.h>
#include <stdlib.h> //rand
#include <assert.h>
#include <unistd.h>

#include <pthread.h>    /* required for pthreads */
#include <semaphore.h>  /* required for semaphores */

/* to compile use -lpthread */
void * producer(void *); 
void * consumer(void *); 

#define HOWMANY_PRODUCERS     80
#define HOWMANY_CONSUMERS     10
// SIZE * HOWMANY_PRODUCERS should be divisible by HOMANY_CONSUMERS


/* functions */ 
//int insert_item(int); // add the value to buffer 
//int remove_item(void); // remove the item from the buffer

/**************************************************************
 *                       Stuff for the buffer                 *
 **************************************************************/
//#define SIZE 100 
//int buffer[SIZE]; 

#define SIZE 100
int freeSlot = 0; 

// initialize the mutex (static initialization) 
pthread_mutex_t mutex_for_buffer = PTHREAD_MUTEX_INITIALIZER; 
/*************************************************************/

/*************************************************************
*                     Debug stuff                            *
**************************************************************/
//#define DEBUG   1
#ifdef DEBUG
#define PRINT printf 
#else 
#define PRINT(...) 
#endif 

sem_t sFree, sFull; 


/****************************************************************************************************************************
*                     Implementing the linked list and functions related to that                            *
*****************************************************************************************************************************/

typedef struct _BufferNode    // structure of the buffer node
{                                 
    struct _BufferNode* next;
    int data;
}BufferNode,*BufferNode_p,**BufferNode_pp;//Create structure


BufferNode_p allocate_BufferNode(int x)  // Allocate space for the buffer
{                                      
    BufferNode_p p=(BufferNode_p)malloc(sizeof(BufferNode));
    if(!p)
	{
        perror("malloc Error");
        exit(1);
    }   
   p->data=x;
   p->next=NULL;
   return p;
}

void init_list(BufferNode_pp head)   //Initialize the linked list
{
    if(head==NULL)
	{
        return;
    }   
    *head=allocate_BufferNode(0);
}

int is_empty(BufferNode_p head)   //Determine whether the linked list is empty or not
{
    return head->next==NULL?1:0;
}


/****************************************************************************************************************************
*                     Use the already builted linked list as a buffer                           *
*****************************************************************************************************************************/
/**********************************************************************************************************************************************************
*                     Since buffer is unbounded, this work in an INFINITY loop                           *
***********************************************************************************************************************************************************/


/**
 * add the given item to the buffer 
 * do not call if the buffer is full 
 */ 
int insert_item(BufferNode_p head, int val) 
{ 
  assert(!pthread_mutex_lock(&mutex_for_buffer)); 
  {
	BufferNode_p temp=allocate_BufferNode(val);    //insert items to the buffer linked list
    temp->next=head->next;
    head->next=temp;
	freeSlot ++;
  }
  assert(!pthread_mutex_unlock(&mutex_for_buffer)); 
  return 0; // success 
}


/** 
 * remove an item from the buffer 
 * Do not call if there are no items to remove 
 */
void remove_item(BufferNode_p head,int* val)  //remove items from the buffer linked list
{ 
  //int val; 
    assert(!pthread_mutex_lock(&mutex_for_buffer)); 
    {
		if(!is_empty(head))
		{
			BufferNode_p temp=head->next;
			*val=temp->data;
			head->next=temp->next;
			free(temp);
		}
	assert(freeSlot > 0); // there should be something to reomve	
	freeSlot --;
  }
  assert(!pthread_mutex_unlock(&mutex_for_buffer)); 
  //return val; // removed value  
}


/** 
 * produce some value 
 * insert to the buffer using the insert_item function 
 * sleep for a random time 
 * repeat SIZE times
 */
void * producer(void * p) 
{ 
    int i = 0, val; 
    while(i++ < SIZE) 
	{ 
		BufferNode_p head=(BufferNode_p)p;
        val = rand() % 20; // produce value 
		//sem_wait(&sFree); //sem_wait -> Same as wait() operation
        insert_item(head,val);
        //printf("produced the data :%d\n",val);  // print the threads
		sem_post(&sFull); // sem_post -> Same as Signal() operation
		usleep(rand() % 10); 
    }
    PRINT("%s: Done\n", __func__); 
}


/** 
 * remove a value from the buffer 
 * using the remove_item function 
 * consume the item, sleep for random time 
 * repeat 
 */

void * consumer(void *p) 
{ 
	int i = 0, val; 
	BufferNode_p head=(BufferNode_p)p;
	while(i++ < (SIZE * HOWMANY_PRODUCERS/HOWMANY_CONSUMERS)) 
	{ 
		sem_wait(&sFull); // sem_wait -> Same as wait() operation
		remove_item(head, &val); // remove from buffer 
		//printf("consumed the data :%d\n",val); // print the threads
		//sem_post(&sFree); //sem_post -> Same as Signal() operation 
		usleep(rand() % 10); 
	}
	PRINT("%s: Done\n", __func__); 
}


/*
Some important methods that can be used with semaphore in c

sem_init 				-> Initialise the semaphore to some initial value
sem_wait 				-> Same as wait() operation
sem_post 				-> Same as Signal() operation
sem_destroy			    -> Destroy the semaphore to avoid memory leak
pthread_mutex_init 		-> Initialise the mutex
pthread_mutex_lock() 	-> Same as wait() operation
pthread_mutex_unlock() 	-> Same as Signal() operation
pthread_mutex_destroy() -> Destroy the mutex to avoid memory leak

*/



int main() 
{
	pthread_t prods[HOWMANY_PRODUCERS]; 
	pthread_t cons[HOWMANY_CONSUMERS]; 

	pthread_attr_t attr; 
 
	pthread_attr_init(&attr); // set default attributes 

	BufferNode_p head; // initializing the head
	init_list(&head);  // initializing the 


	//initially the buffer is empty (so empty=1) and it is not full (so full=0) and a mutex lock as sm initialized as 1.
	//assert(!sem_init(&sFree, 0/*shared between threads*/, SIZE-1)); 
	assert(!sem_init(&sFull, 0/*shared between threads*/, 0)); 
	 
	int i; 
	for(i=0; i<HOWMANY_PRODUCERS; i++) 
		pthread_create(&prods[i], &attr, producer, (void*)head); // create producers thread which will be executing till it’s in producer method

	for(i=0; i<HOWMANY_CONSUMERS; i++) 
		pthread_create(&cons[i], &attr, consumer, (void*)head); // create new thread which will be executing till it’s in Consumer method

	void *dummy; 
	for(i=0; i<HOWMANY_PRODUCERS; i++) 
		pthread_join(prods[i], &dummy);  // ensures that the next thread completes its execution only after the completion of the given producers thread

	for(i=0; i<HOWMANY_CONSUMERS; i++) 
		pthread_join(cons[i], &dummy);  // ensures that the next thread completes its execution only after the completion of the given Consumer thread


	// Since this is unbounded this function is not going to work
	 printf("%s: all threads exited (solution %s)\n",
	 __func__, 
	 freeSlot == 0 ? "worked!" : "failed"); 

	pthread_mutex_destroy(&mutex_for_buffer); 
	//sem_destroy(&sFree); 
	sem_destroy(&sFull);   
}