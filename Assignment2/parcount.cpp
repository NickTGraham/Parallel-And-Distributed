/* File: parcount.cpp
 * Author: Nicholas Graham
 * Date: January 29, 2017
 * version: 1.0
 * Description: Assignment Two: comparing locks
 */

#include "parcount.h"
#include "locks.h"

/* Global Variables */
std::atomic_bool start;         // boolean to sync the start of the threads
std::mutex stageTwoMutex;       // Mutex to syncing during stage two
std::mutex stageThreeMutex;     // Mutex for syncing during stage three

int main(int argc, char *argv[]) {

    /* Set up default number of threads and increments */
    int numberOfThreads = 4;
    int counterNumber = 10000;

    /* Get the Command line parameters */
    for (int i = 1; i < argc-1; i++) {
        if (strcmp(argv[i], "-t") == 0) {
            numberOfThreads = atoi(argv[i+1]);
        }
        if (strcmp(argv[i], "-i") == 0) {
            counterNumber = atoi(argv[i+1]);
        }
    }

    printf("%d, %d, ", numberOfThreads,  counterNumber);
    start = false;
    std::thread *threads = new std::thread[numberOfThreads];
    time startTime;
    time endTime;
    time_delta duration;

    /* C++ Mutex */
    int counter = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (cppMutex, &counter, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "C++ Mutex," << counter << "," << duration.count() << std::endl;
    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f, ", ((double) counter) / duration.count() );
    fflush(stdout);

    /* Naive TAS */
    counter = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (TASLock, &counter, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "Test and Set," << counter << "," << duration.count() << std::endl;
    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f, ", ((double) counter) / duration.count() );
    fflush(stdout);

    /* TAS with Backoff */
    counter = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (TASLockBackoff, &counter, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "Test and Set Backoff," << counter << "," << duration.count() << std::endl;
    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f, ", ((double) counter) / duration.count() );
    fflush(stdout);

    /* TATAS */
    counter = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (TATASLock, &counter, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "Test and Test and Set," << counter << "," << duration.count() << std::endl;

    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f, ", ((double) counter) / duration.count() );
    fflush(stdout);

    /* Ticket Lock */
    counter = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (TicketLock, &counter, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "TicketLock," << counter << "," << duration.count() << std::endl;

    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f, ", ((double) counter) / duration.count() );
    fflush(stdout);

    /* Ticket Lock Backoff*/
    counter = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (TicketLockBackoff, &counter, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "TicketLock Backoff," << counter << "," << duration.count() << std::endl;

    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f, ", ((double) counter) / duration.count() );
    fflush(stdout);

    /* MCS Lock*/
    counter = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (MCSLock, &counter, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "MCS," << counter << "," << duration.count() << std::endl;

    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f, ", ((double) counter) / duration.count() );
    fflush(stdout);

    /* MCS K42 Lock */
    counter = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (K42MCSLock, &counter, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "MCS K42," << counter << "," << duration.count() << std::endl;

    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f, ", ((double) counter) / duration.count() );
    fflush(stdout);

    /* CLH Lock */
    counter = 0;

    qnodeCLH nodes[numberOfThreads];
    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (CLHLock, &counter, counterNumber, &(nodes[i]));
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "CLH Lock," << counter << "," << duration.count() << std::endl;

    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f, ", ((double) counter) / duration.count() );
    fflush(stdout);

    /* CLH K42 Lock */
    counter = 0;

    LockCLHK42 clh_k42_lock(numberOfThreads);

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (K42CLHLock, &counter, counterNumber, i, &clh_k42_lock);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false;

    //std::cout << "CLH K42 Lock," << counter << "," << duration.count() << std::endl;
    if (counter != numberOfThreads * counterNumber) {
        fprintf(stderr, "Oh No!\n");
    }

    printf("%f \n", ((double) counter) / duration.count() );
}




void cppMutex (int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        stageTwoMutex.lock();
        *counter = *counter + 1;
        stageTwoMutex.unlock();
    }
}

LockTAS test_and_set_lock;
void TASLock (int *counter, int number) {
    while(!start.load());

    for (int i = 0; i < number; i++) {
        test_and_set_lock.acquire();
        *counter = *counter + 1;
        test_and_set_lock.release();
    }
}

void TASLockBackoff (int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        test_and_set_lock.acquireBackoff(1, 1000, 2);
        *counter = *counter + 1;
        test_and_set_lock.release();
    }
}

void TATASLock (int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        test_and_set_lock.acquireTATAS();
        *counter = *counter + 1;
        test_and_set_lock.releaseTATAS();
    }
}

LockTicket ticket_lock;

void TicketLock (int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        ticket_lock.acquire();
        *counter = *counter + 1;
        ticket_lock.release();
    }
}

void TicketLockBackoff (int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        ticket_lock.acquireBackoff(1);
        *counter = *counter + 1;
        ticket_lock.release();
    }
}

LockMCS mcs_lock;
void MCSLock (int *counter, int number) {
    while (!start.load());

    qnodeMCS mine;
    //printf("...");
    for (int i = 0; i < number; i++) {
        mcs_lock.acquire(&mine);
        *counter = *counter + 1;
        mcs_lock.release(&mine);
    }
}

LockMSCK42 mcs_k42_lock;
void K42MCSLock (int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        mcs_k42_lock.acquire();
        *counter = *counter + 1;
        mcs_k42_lock.release();
    }
}

LockCLH clh_lock;

void CLHLock (int *counter, int number, qnodeCLH *mine) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        clh_lock.acquire(mine);
        *counter = *counter + 1;
        clh_lock.release(&mine);
        //fprintf(stderr, "...");
    }
}

void K42CLHLock (int *counter, int number, int thread, LockCLHK42 *lock) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        lock->acquire(thread);
        *counter = *counter + 1;
        lock->release();
        //fprintf(stderr, "... [%d]", thread);
    }
}
