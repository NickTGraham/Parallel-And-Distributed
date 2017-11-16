/* File: locks.h
 * Author: Nicholas Graham
 * Date: Febuary 13, 2017
 * version: 1.0
 * Description: Assignment Two: comparing locks
 */

#ifndef __locks_include__
#define __locks_include__
#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <ctime>
#include <ratio>
#include <chrono>

void mySleep (double sec);

class LockTAS {
    private:
        std::atomic_bool g = {false};
        std::atomic_flag f = ATOMIC_FLAG_INIT;
    public:
        void acquire();
        void release();
        void acquireTATAS();
        void releaseTATAS();
        void acquireBackoff(double base, double limit, double mult);
};

class LockTicket {
    private:
        std::atomic_int next_ticket = {0};
        std::atomic_int now_serving = {0};
    public:
        void acquire();
        void release();
        void acquireBackoff(double base);
};

class qnodeMCS {
    public:
        std::atomic_bool waiting;
        std::atomic<qnodeMCS*> next;
};

class LockMCS {
    private:
        std::atomic<qnodeMCS*> tail = {NULL};
    public:
        void acquire(qnodeMCS *p);
        void release(qnodeMCS *p);
};

class qnodeK42 {
    public:
        std::atomic<qnodeK42*> tail;
        std::atomic<qnodeK42*> next;
};

class LockMSCK42 {
    private:
        qnodeK42 q = {{NULL}, {NULL}};
    public:
        void acquire();
        void release();
};

class qnodeCLH {
    public:
        std::atomic_bool succ_must_wait;
        char padding[64];
        qnodeCLH *prev;
};

class LockCLH {
    private:
        qnodeCLH dummy = {{false}, NULL, NULL};
        std::atomic<qnodeCLH*> tail = {&dummy};
    public:
        void acquire(qnodeCLH *p);
        void release(qnodeCLH **pp);
};

class qnodeCLHK42 {
    public:
        std::atomic_bool succ_must_wait;
};

class LockCLHK42 {
    private:
        qnodeCLHK42 dummy = {{false}};
        std::atomic<qnodeCLHK42*> tail = {&dummy};
        std::atomic<qnodeCLHK42*> head;
        qnodeCLHK42 *initial_thread_qnode;
        qnodeCLHK42 **initial_thread_ptr;
    public:
        LockCLHK42(int numberOfThreads);
        void acquire(int id);
        void release();
};

#endif
