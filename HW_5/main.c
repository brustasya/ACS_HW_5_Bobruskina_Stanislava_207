#include "declaration.h"

int main(int k, char** args)
{
    pthread_t barber[NUM_BARB],customer[MAX_CUST]; //Thread declaration
    int i,status=0;
    /*Semaphore initialization*/
    sem_init(&customers,0,0);
    sem_init(&barbers,0,0);
    sem_init(&mutex,0,1);
    /*Barber thread initialization*/
    printf("!!Barber Shop Opens!!\n");
    for(i=0;i<NUM_BARB;i++)                     //Creation of 2 Barber Threads
    {
        status=pthread_create(&barber[i],NULL,(void *)barberThread,(void*)&i);
        sleep(1);
        if(status!=0)
            perror("No Barber Present... Sorry!!\n");
    }
    /*Customer thread initialization*/
    for(i=0;i<MAX_CUST;i++)                     //Creation of Customer Threads
    {
        status=pthread_create(&customer[i],NULL,(void *)customerThread,(void*)&i);
        wait();                   //Create customers in random interval
        if(status!=0)
            perror("No Customers Yet!!!\n");
    }
    for(i=0;i<MAX_CUST;i++)        //Waiting till all customers are dealt with
        pthread_join(customer[i],NULL);
    printf("!!Barber Shop Closes!!\n");
    exit(EXIT_SUCCESS);  //Exit abandoning infinite loop of barber thread
}

void customerThread(void *tmp)  /*Customer Process*/
{
    int mySeat, B;
    sem_wait(&mutex);  //Lock mutex to protect seat changes
    count++;           //Arrival of customer
    printf("Customer-%d[Id:%d] Entered Shop. ",count,pthread_self());
    if(numberOfFreeSeats > 0)
    {
        --numberOfFreeSeats;           //Sit on chairs on waiting room
        printf("Customer-%d Sits In Waiting Room.\n",count);
        sitHereNext = (++sitHereNext) % MAX_CHAIRS;  //Choose a vacant chair to sit
        mySeat = sitHereNext;
        seatPocket[mySeat] = count;
        sem_post(&mutex);                  //Release the seat change mutex
        sem_post(&barbers);                //Wake up one barber
        sem_wait(&customers);              //Join queue of sleeping customers
        sem_wait(&mutex);                  //Lock mutex to protect seat changes
        B = seatPocket[mySeat];    //Barber replaces customer PID with his own PID
        numberOfFreeSeats++;             //Stand Up and Go to Barber Room
        sem_post(&mutex);                        //Release the seat change mutex
        /*Customer is having hair cut by barber 'B'*/
    }
    else
    {
        sem_post(&mutex);  //Release the mutex and customer leaves without haircut
        printf("Customer-%d Finds No Seat & Leaves.\n",count);
    }
    pthread_exit(0);
}

void barberThread(void *tmp)        /*Barber Process*/
{
    int index = *(int *)(tmp);
    int myNext, C;
    printf("Barber-%d[Id:%d] Joins Shop. ",index,pthread_self());
    while(1)            /*Infinite loop*/
    {
        printf("Barber-%d Gone To Sleep.\n",index);
        sem_wait(&barbers);          //Join queue of sleeping barbers
        sem_wait(&mutex);            //Lock mutex to protect seat changes
        serveMeNext = (++serveMeNext) % MAX_CHAIRS;  //Select next customer
        myNext = serveMeNext;
        C = seatPocket[myNext];                  //Get selected customer's PID
        seatPocket[myNext] = pthread_self();     //Leave own PID for customer
        sem_post(&mutex);
        sem_post(&customers);         //Call selected customer
        /*Barber is cutting hair of customer 'C'*/
        printf("Barber-%d Wakes Up & Is Cutting Hair Of Customer-%d.\n",index,C);
        sleep(CUT_TIME);
        printf("Barber-%d Finishes. ",index);
    }
}

void wait()     /*Generates random number between 50000 to 250000*/
{
    int x = rand() % (250000 - 50000 + 1) + 50000;
    srand(time(NULL));
    usleep(x);     //usleep halts execution in specified miliseconds
}