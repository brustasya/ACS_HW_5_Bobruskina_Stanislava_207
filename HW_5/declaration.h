#include <unistd.h>          //Provides API for POSIX(or UNIX) OS for system calls
#include <stdio.h>           //Standard I/O Routines
#include <stdlib.h>          //For exit() and rand()
#include <pthread.h>         //Threading APIs
#include <semaphore.h>       //Semaphore APIs
#define MAX_CHAIRS 10        //No. of chairs in waiting room
#define CUT_TIME 1           //Hair Cutting Time 1 second
#define NUM_BARB 2           //No. of barbers
#define MAX_CUST 30          //Maximum no. of customers for simulation

sem_t customers;                 //Semaphore
sem_t barbers;                   //Semaphore
sem_t mutex;                     //Semaphore for providing mutially exclusive access
int numberOfFreeSeats = MAX_CHAIRS;   //Counter for Vacant seats in waiting room
int seatPocket[MAX_CHAIRS];           //To exchange pid between customer and barber
int sitHereNext = 0;                  //Index for next legitimate seat
int serveMeNext = 0;                  //Index to choose a candidate for cutting hair
static int count = 0;                 //Counter of No. of customers
void barberThread(void *tmp);         //Thread Function
void customerThread(void *tmp);       //Thread Function
void wait();      