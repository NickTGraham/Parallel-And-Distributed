/* File: parcount.h
 * Author: Nicholas Graham
 * Date: Febuary 13, 2017
 * version: 1.0
 * Description: Assignment Two: comparing locks
 */

#ifndef __parcount_include__
#define __parcount_include__

// Includes
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
#include "locks.h"

#define time std::chrono::high_resolution_clock::time_point
#define now(void) std::chrono::high_resolution_clock::now()
#define time_delta std::chrono::duration<double>
#define time_duration(s, e) std::chrono::duration_cast<std::chrono::duration<double>>(e - s)

void cppMutex (int *counter, int number);
void TASLock (int *counter, int number);
void TASLockBackoff (int *counter, int number);
void TATASLock (int *counter, int number);
void TicketLock (int *counter, int number);
void TicketLockBackoff (int *counter, int number);
void MCSLock (int *counter, int number);
void K42MCSLock (int *counter, int number);
void CLHLock (int *counter, int number, qnodeCLH *mine);
void K42CLHLock (int *counter, int number, int thread, LockCLHK42 *lock);

#endif
