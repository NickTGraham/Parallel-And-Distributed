/* File: parcount.cpp
 * Author: Nicholas Graham
 * Date: January 29, 2017
 * version: 1.0
 * Description: Assignment One: look at different methods of syncronization
 * using C++ built in threads.
 */

#include "parcount.h"

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

    /* Display given parameters, and expected output */
    std::cout << "t " << numberOfThreads << " i " << counterNumber << "\n";
    std::cout << "Expected Result: " << numberOfThreads * counterNumber << "\n";

    /* Initialize the thread parameters */
    start = false;
    std::thread *threads = new std::thread[numberOfThreads];
    time startTime;
    time endTime;
    time_delta duration;
    /* Executes 5 stages */

    /* 1: No syncronization */
    int counterOne = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (stageOne, &counterOne, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false; // reset start variable for next time
    //std::cout << "Method One: Result [" << counterOne << "] Time [" << duration.count() << " sec]" <<std::endl;
    std::cout << "Method One," << counterOne << "," << duration.count() << std::endl;
    // 2: Mutex with Lock and Unlock
    int counterTwo = 0;

    /* Create and start the threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (stageTwo, &counterTwo, counterNumber);
    }

    startTime = now();
    start = true; // tell threads to start executing

    /* Wait on threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false; // reset start variable
    //std::cout << "Method Two: Result [" << counterTwo << "] Time [" << duration.count() << " sec]" <<std::endl;
    std::cout << "Method Two," << counterTwo << "," << duration.count() << std::endl;
    // 3: Mutex using local lock-guard
    int counterThree = 0;

    /* Create and start threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (stageThree, &counterThree, counterNumber);
    }

    startTime = now();
    start = true; // Tell threads to start executing

    /* Wait for threads to finish */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false; // reset start
    //std::cout << "Method Three: Result [" << counterThree << "] Time [" << duration.count() << " sec]" <<std::endl;
    std::cout << "Method Three," << counterThree << "," << duration.count() << std::endl;

    // 4: Counter as an atomic int, using the fetch_add operations
    std::atomic_int counterFour;
    counterFour = 0;

    /* Create and start threads */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i] = std::thread (stageFour, &counterFour, counterNumber);
    }

    startTime = now();
    start = true; // Tell threads to start executing

    /* Wait for threads to finish */

    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false; // reset start
    //std::cout << "Method Four: Result [" << counterFour << "] Time [" << duration.count() << " sec]" <<std::endl;
    std::cout << "Method Four," << counterFour << "," << duration.count() << std::endl;

    // 5: Using Combining
    int counterFive = 0;
    int *subCounters = new int[numberOfThreads];

    /* Create and start threads */
    for (int i = 0; i < numberOfThreads; i++) {
        subCounters[i] = 0;
        threads[i] = std::thread (stageFive, &(subCounters[i]), counterNumber);
    }

    startTime = now();
    start = true; // Tell threads to start executing

    /* Wait for threads to finish, and add results from each to the total */
    for (int i = 0; i < numberOfThreads; i++) {
        threads[i].join();
        counterFive += subCounters[i];
    }
    endTime = now();
    duration = time_duration(startTime, endTime);
    start = false; // reset start
    //std::cout << "Method Five: Result [" << counterFive << "] Time [" << duration.count() << " sec]" <<std::endl;
    std::cout << "Method Five," << counterFive << "," << duration.count() << std::endl;

}

void stageOne (int *counter, int number) {
    while(!start.load());

    for (int i = 0; i < number; i++) {
        *counter = *counter + 1;
    }
}

void stageTwo (int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        stageTwoMutex.lock();
        *counter = *counter + 1;
        stageTwoMutex.unlock();
    }
}

void stageThree (int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        std::lock_guard<std::mutex> lock(stageThreeMutex);
        *counter = *counter + 1;
    }
}

void stageFour (std::atomic_int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        (*counter).fetch_add(1);
    }
}

void stageFive (int *counter, int number) {
    while (!start.load());

    for (int i = 0; i < number; i++) {
        (*counter)++;
    }
}
void tears(int *i) {
    *i = *i + 1;
    std::cout << *i << " Nothing but pain\n";
}
