/* File: locks.h
 * Author: Nicholas Graham
 * Date: Febuary 13, 2017
 * version: 1.0
 * Description: Assignment Two: comparing locks
 */

#include "locks.h"

#define min(a,b) (a < b)?a:b

void mySleep (double sec) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int)sec));
}

void LockTAS::acquire() {
    while (f.test_and_set(std::memory_order_acquire));
}

void LockTAS::release() {
    f.clear(std::memory_order_release);
}

void LockTAS::acquireTATAS() {
    while (g.exchange(true, std::memory_order_acquire)) {
        while (g.load(std::memory_order_relaxed));
    }
}
void LockTAS::releaseTATAS() {
    g.store(false, std::memory_order_release);
}

void LockTAS::acquireBackoff (double base, double limit, double mult) {
    double delay = base;
    while (f.test_and_set(std::memory_order_acquire)) {
        mySleep(base);
        min(limit, delay * mult );
    }
}

void LockTicket::acquire() {
    int my_ticket = next_ticket.fetch_add(1, std::memory_order_acquire);

    while(true) {
        int ns = now_serving.load(); //mem order ???

        if (ns == my_ticket){
            break;
        }
    }
}

void LockTicket::release() {
    int t = now_serving + 1;
    now_serving.store(t, std::memory_order_release);
}

void LockTicket::acquireBackoff(double base) {
    int my_ticket = next_ticket.fetch_add(1, std::memory_order_acquire);
    while(true) {
        int ns = now_serving.load(); //mem order ???

        if (ns == my_ticket){
            break;
        }
        mySleep(base * (my_ticket - ns));
    }
}

void LockMCS::acquire(qnodeMCS *p) {
    p->next.store(NULL);
    p->waiting.store(true);
    qnodeMCS* prev = tail.exchange(p);
    if (prev != NULL) {
        prev->next.store(p);
        while (p->waiting.load(std::memory_order_acquire));
    }
}

void LockMCS::release(qnodeMCS *p) {
    qnodeMCS *succ = p->next.load(std::memory_order_release);
    //p->waiting.store(false);
    if (succ == NULL) {
        qnodeMCS *tmp = p;
        if (tail.compare_exchange_strong(tmp, NULL)) {
            return;
        }
        while (succ == NULL) {
            succ = p->next.load();
        }
        succ->waiting.store(false);
    }
    else {
        succ->waiting.store(false);
    }
}

qnodeK42 *waiting = (qnodeK42*) 1;

void LockMSCK42::acquire() {
    while (true) {
        qnodeK42 *prev = q.tail.load();
        if (prev == NULL) {
            qnodeK42 *tmp = NULL;
            if (std::atomic_compare_exchange_strong(&q.tail, &tmp, &q)) {
                break;
            }
        }
        else {
            qnodeK42 n = {{waiting}, {NULL}};
            //if (q.tail.compare_exchange_strong(prev, &n)) {
            qnodeK42 *tmp = prev;
            if (std::atomic_compare_exchange_strong(&q.tail, &tmp, &n)) {
                prev->next.store(&n);
                while (n.tail.load() == waiting);
                qnodeK42 *succ = n.next.load();
                if (succ == NULL) {
                    q.next.store(NULL);
                    //if(!q.tail.compare_exchange_strong(&n, &q)) {
                    tmp = &n;
                    if (!std::atomic_compare_exchange_strong(&q.tail, &tmp, &q)) {
                        while (succ == NULL) {
                            succ = n.next.load();
                        }
                        q.next.store(succ);
                    }
                    break;
                }
                else {
                    q.next.store(succ);
                    break;
                }
            }

        }
    }
}

void LockMSCK42::release() {
    qnodeK42 *succ = q.next.load(std::memory_order_release);
    if (succ == NULL) {
        //if (q.tail.compare_exchange_strong(&q, NULL)) {
        qnodeK42 *tmp = &q;
        qnodeK42 *tmp2 = NULL;
        if (std::atomic_compare_exchange_strong(&q.tail, &tmp, tmp2)) {
            return;
        }
        else {
            while (succ == NULL) {
                succ = q.next.load(std::memory_order_release);
            }
        }
    }
    succ->tail.store(NULL);
}

void LockCLH::acquire(qnodeCLH *p) {
    p->succ_must_wait = true;
    qnodeCLH *prev = p->prev = tail.exchange(p);
    while (prev->succ_must_wait.load());
}

void LockCLH::release(qnodeCLH **pp) {
    qnodeCLH *pred = (*pp)->prev;
    (*pp)->succ_must_wait.store(false, std::memory_order_release);
    *pp = pred;
}

LockCLHK42::LockCLHK42(int numberOfThreads) {
    initial_thread_qnode = new qnodeCLHK42[numberOfThreads];
    initial_thread_ptr = new qnodeCLHK42*[numberOfThreads];
    for (int i = 0; i<numberOfThreads; i++) {
        initial_thread_ptr[i] = &(initial_thread_qnode[i]);
    }
}

void LockCLHK42::acquire(int id) {
    qnodeCLHK42 *p = initial_thread_ptr[id];
    p->succ_must_wait = true;
    qnodeCLHK42 *pred = tail.exchange(p);
    while (pred->succ_must_wait.load());
    head.store(p, std::memory_order_acquire);
    initial_thread_ptr[id] = pred;
}

void LockCLHK42::release() {
    head.load()->succ_must_wait.store(false, std::memory_order_release);
}
